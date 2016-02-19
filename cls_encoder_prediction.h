#include "lib_import.h"

template<typename TIN,typename TOUT,typename FUNC>
class cls_encoder_prediction
{
private:
	TOUT *data_output;
	const TIN *data_input;
	int num;

	FUNC fn_pred;

public:
	cls_encoder_prediction(const TIN *data,int a_num,FUNC a_fn_pred) : data_input(data)
	{
		num = a_num;
		fn_pred = a_fn_pred;
		data_output = new TOUT[a_num];
	}

	~cls_encoder_prediction()
	{
		delete[] data_output;
	}

	TOUT* encode(int size_pred)
	{
		data_output[0] = data_input[0];
		for(int cnt=0;cnt<size_pred;cnt++)
		{
			data_output[cnt] = data_input[cnt];
		}

		for(int cnt=size_pred;cnt<num;cnt++)
		{
			data_output[cnt] = data_input[cnt] - fn_pred(data_input + cnt,size_pred);
		}

		return data_output;
	}
};