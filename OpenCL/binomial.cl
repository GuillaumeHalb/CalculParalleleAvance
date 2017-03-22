    __kernel void binomial(
            __global float* option,
            __global float* model,
            __global float* data,
            const int depth,
            const int size,
            const int root_i,
            const int reverse)
    {

        unsigned int group0 = get_group_id(0);
        unsigned int group1 = get_group_id(1);
        unsigned int global0 = get_global_id(0);
        unsigned int global1 = get_global_id(1);
        unsigned int local0 = get_local_id(0);
        unsigned int local1 = get_local_id(1);

        unsigned int root_j = 0;

        unsigned int line = 0;
        unsigned int column = 0;


        if (group0 == 0) {
            root_j = 0;
        } else {
            root_j = root_i;
        }


        if (reverse == 0) {

            line = (size + root_i - 1) - local1 ;
            column = root_j + local0;

            barrier(CLK_LOCAL_MEM_FENCE | CLK_GLOBAL_MEM_FENCE);


            if (column < (root_j + size - local1)  && (line < depth)) {
                float stock = option[0] * pow(model[3], line  - column) * pow(model[2], column);

                float payoff = option[1]-stock > 0 ? option[1]-stock : 0;

                float v = model[6] * (model[4] * data[column + 1 + (line + 1)*(line+2)/2] +
                                      model[5] * data[column + (line + 1)*(line+2)/2]);
                float value = fmax(payoff, v);
                data[column + line*(line+1)/2] = value;
            }

            barrier(CLK_LOCAL_MEM_FENCE | CLK_GLOBAL_MEM_FENCE);

        } else if (reverse == 1) {


            root_j = root_i-(size+1);

            line = (root_i) - local1 ;
            column = root_j - local0;


            barrier(CLK_LOCAL_MEM_FENCE | CLK_GLOBAL_MEM_FENCE);

            if (column <= line  && line < depth) {
                float stock = option[0] * pow(model[3], line  - column) * pow(model[2], column);

                float payoff = option[1]-stock > 0 ? option[1]-stock : 0;
                float v = model[6] * (model[4] * data[column + 1 + (line + 1)*(line+2)/2] +
                                      model[5] * data[column + (line + 1)*(line+2)/2]);
                float value = fmax(payoff, v);
                data[column + line*(line+1)/2] = value;
            }

            barrier(CLK_LOCAL_MEM_FENCE | CLK_GLOBAL_MEM_FENCE);


        } else {
            line = size - local1 ;
            column = local0;

            barrier(CLK_LOCAL_MEM_FENCE | CLK_GLOBAL_MEM_FENCE);


            if (column <= (size - local1)  && (line < depth)) {
                float stock = option[0] * pow(model[3], line  - column) * pow(model[2], column);

                float payoff = option[1]-stock > 0 ? option[1]-stock : 0;
                float v = model[6] * (model[4] * data[column + 1 + (line + 1)*(line+2)/2] +
                                      model[5] * data[column + (line + 1)*(line+2)/2]);
                float value = fmax(payoff, v);
                data[column + line*(line+1)/2] = value;
            }

            barrier(CLK_LOCAL_MEM_FENCE | CLK_GLOBAL_MEM_FENCE);

        }
    }