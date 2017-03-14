__kernel void binomial(
    __global int depth,
    __global int root_i,
    __global int root_j,
    __global float* option
    __global float* model
    __global float* data)
{

    __local float stock_root = option[0] * pow(model[3], root_i - root_j) * pow(model[2], root_j);
       __local int line = get_global_id(0);
       __local int column = get_globale_id(1);
       for (line = depth + root_i ; line >= root_i ; line--)
       {
           __local float stock = stock_root * pow(model[3], (line - root_i));
           for (column = root_j ; column <= (line - root_i) + root_j ; column++)
           {
               if (column <= line) {
                   __local float payoff = option[K] - stock;
                   if (payoff < 0) {
                       payoff = 0;
                   }

                   __local float value = fmax(payoff, model[6]*(model[4]*data[column + 1 + (line + 1)*(line + 2)/2]
                   + model[5] * data[column + (line + 1)*(line + 2)/2]));
                   data[column + line*(line+1)/2] = value;
                   stock *= model[2] / model[3];
               }
           }
       }

}