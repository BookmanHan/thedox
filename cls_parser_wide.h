
template<typename T>
class cls_parser_wide
	:public cls_a_lalr_dealing_void_wide<T>
{
protected:
	cls_lalr yacc;
	cls_lalr_frame_wide<T>* frame;

public:
	cls_parser_wide()
	{
		frame = null;
	}

	~cls_parser_wide()
	{
		if (frame != null)
			delete frame;
	}

	virtual T* run(const wstring& str_context) = 0;
	virtual wstring operator()(const wstring& item) = 0;
	virtual bool check_alpha(const wchar_t c) = 0;
	virtual bool check_digit(const wchar_t c) = 0;
	virtual void operator()(vector<cls_lalr_frame_item_wide<T> > & now,
		int next_status,
		cls_lalr_frame_item_wide<T> &re,
		vector<cls_lalr_frame_item_wide<T> >& stack) = 0;

	virtual void spilt(wstring str,vector<wstring>& re)
	{
		int status = 0;
		int pos = 0;

		wstring::iterator itor;

		int cnt = 0;
		for(cnt=0,itor=str.begin();
			itor!=str.end();
			itor++,cnt++)
		{
			switch(status)
			{
			case 0:
				{
					if (check_digit(*itor))
					{
						status = 2;
					}
					else if (check_alpha(*itor) || *itor == '_')
					{
						status = 1;
					}
					else if (*itor == '\"' )
					{
						status = 3;
						re.push_back(str.substr(cnt,1));
						pos = cnt+1;
					}
					else if (*itor == '\'')
					{
						status  = 4;
						re.push_back(str.substr(cnt,1));
						pos = cnt+1;
					}
					else
					{
						status = 0;

						re.push_back(str.substr(cnt,1));
						pos = cnt+1;
					}
				}
				break;

			case 1:
				{
					if (check_alpha(*itor) || check_digit(*itor) || *itor=='_')
					{
						;
					}
					else
					{
						status = 0;
						re.push_back(str.substr(pos,cnt-pos));
						re.push_back(str.substr(cnt,1));
						pos = cnt + 1;
					}
				}
				break;

			case 2:
				{
					if (check_digit(*itor) || *itor=='.')
					{
						;
					}
					else
					{
						status = 0;
						re.push_back(str.substr(pos,cnt-pos));
						re.push_back(str.substr(cnt,1));
						pos = cnt + 1;
					}
				}
				break;

			case 3:
				{
					if (*itor == '\"')
					{
						re.push_back(str.substr(pos,cnt-pos));
						re.push_back(str.substr(cnt,1));
						pos = cnt+1;
						status = 0;
					}
				}
				break;

			case 4:
				{
					if (*itor == '\'')
					{
						re.push_back(str.substr(pos,cnt-pos));
						re.push_back(str.substr(cnt,1));
						pos = cnt+1;
						status = 0;
					}
				}
				break;
			}
		}

		re.push_back(str.substr(pos,str.size()-pos));
	}
};
