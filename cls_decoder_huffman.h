#include "lib_import.h"

template<class T>
class cls_decoder_huffman
{
private:
	unsigned char * data;
	int num;

	T *output;

	struct itag_decode_automata_node
	{
		int next_1;
		int next_0;
		T cur;
	};

	vector<itag_decode_automata_node> codes;

	void i_make_decode_automata(vector<pair<string,T> > &cur_v,int cur_node)
	{
		if (cur_v.size() <= 1)
		{
			codes[cur_node].cur = cur_v[0].second;
			codes[cur_node].next_1 = -1;
			codes[cur_node].next_0 = -1;

			return;
		}

		vector<pair<string,T> > v_0;
		vector<pair<string,T> > v_1;

		for(int cnt=0;cnt<cur_v.size();cnt++)
		{
			if (cur_v[cnt].first[0] == '0')
			{
				v_0.push_back(make_pair(cur_v[cnt].first.substr(1,cur_v[cnt].first.length()-1),cur_v[cnt].second));
			}
			else
			{
				v_1.push_back(make_pair(cur_v[cnt].first.substr(1,cur_v[cnt].first.length()-1),cur_v[cnt].second));
			}
		}

		itag_decode_automata_node tmp;
		codes[cur_node].next_0 = codes.size();
		codes.push_back(tmp);
		i_make_decode_automata(v_0,codes[cur_node].next_0);
		codes[cur_node].next_1 = codes.size();
		codes.push_back(tmp);
		i_make_decode_automata(v_1,codes[cur_node].next_1);

		return;
	}

	void make_decode_automata(map<T,string> a_codes)
	{
		vector<pair<string,T> > v_c;

		#ifdef __USING_GCC__
		typename map<T,string>::iterator itor = a_codes.begin();
		#else
		map<T,string>::iterator itor = a_codes.begin();
		#endif

		while(itor != a_codes.end())
		{
			v_c.push_back(make_pair(itor->second,itor->first));
			itor++;
		}

		codes.resize(1);
		i_make_decode_automata(v_c,0);
	}

public:
	cls_decoder_huffman(unsigned char*a_data,int a_num,map<T,string> a_codes)
	{
		data = a_data;
		num = a_num;

		output = new T[a_num*sizeof(T)*10];
		make_decode_automata(a_codes);
	}

	~cls_decoder_huffman()
	{
		delete[] output;
	}

	pair<T*,int> decode()
	{
		unsigned char*pin = data;
		int nin = 0;
		int automata_pos = 0;
		int nout = 0;

		for(int cnt=0;cnt<num;cnt++)
		{
			for(int nin=0;nin<8;nin++)
			{
				if ((pin[cnt] & (1<<nin)) == 0)
				{
					automata_pos = codes[automata_pos].next_0;
				}
				else
				{
					automata_pos = codes[automata_pos].next_1;
				}

				if (codes[automata_pos].next_0 == -1 && codes[automata_pos].next_1 == -1)
				{
					output[nout++] = codes[automata_pos].cur;
					automata_pos = 0;
				}
			}
		}

		return make_pair(output,nout);
	}
};
