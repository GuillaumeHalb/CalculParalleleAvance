__kernel void binomial(
    __global float* option,
    __global float* model,
    __global float* data,
    const int depth)
{
    unsigned int column = get_local_id(0);
    unsigned int line = depth - 1 - get_global_id(1);


    barrier(CLK_GLOBAL_MEM_FENCE);

    if (column <= line) {
        printf("line: %i, column: %i \n", line, column);
        float stock = option[0] * pow(model[3], line - column)*pow(model[2], column);
        float payoff = option[1] - stock > 0.0 ? option[0] - stock : 0.0;
        data[column + line*(line + 1)/2] = fmax(payoff, model[6]*
            (model[4]*data[column + 1 + (line + 1)*(line + 2)/2] + model[5] * data[column + (line + 1)*(line + 2)/2]));
        column += 1;
    } else {
        column = 0;
        line -= 1;
    }

    barrier(CLK_GLOBAL_MEM_FENCE);


}



