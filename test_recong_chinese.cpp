#include "lib_import.h"
#include "appc_print_spilt.h"
#include "appc_sky_line.h"
#include "appc_cell_recognition.h"
#include "appc_recong_chinese.h"
#include "appc_recongniton_chinese_pact.h"

void test_appc_recong_chinese()
{
	while(true)
	{
		int n_choose;
		cout<<"\n1.加载网络\n2.重新训练网络\n3.校验顺序。"<<endl;
		cin>>n_choose;

		try
		{
			cout<<"输入切分文件名：";
			string str;
			cin>>str;

			appc_print_spilt spilter("D:\\trys\\recog\\" + str + ".bmp","D:\\trys\\recog\\text\\");
			appc_recongntion_chinese reconger("D:\\trys\\recog\\chinese_mixed\\",200,100);

			if (n_choose == 2)
			{
				cout<<"请输入训练次数："<<endl;
				int n_times;
				cin>>n_times;
				reconger.train(n_times);
				reconger.save_net("D:\\trys\\recog\\chinese_mixed\\chinese.net");
			}
			else if (n_choose == 1)
			{
				reconger.load_net("D:\\trys\\recog\\chinese_mixed\\chinese.net");
			}
			else if (n_choose == 3)
			{
				reconger.valid_order();
				return;
			}

			//reconger.inner_try();

			list<cls_image_ot<unsigned> > lst_img_spilt;
			lst_img_spilt.clear();
			spilter.run(lst_img_spilt,false);

			int cnt=0;
			list<cls_image_ot<unsigned> >::iterator itor;
			for(itor = lst_img_spilt.begin();itor != lst_img_spilt.end();itor++)
			{
				try
				{
					wchar_t id = reconger.recognition(*itor);
					//a_save(*itor,string("D:\\trys\\recog\\text\\") + cnt + string(".bmp"));
//					a_save(*itor,wstring(L"D:\\trys\\recog\\text\\") + a_trans_wchar_to_wstr(id) + wstring(L".bmp"));
//
//					wcout.imbue(locale("chs"));
//					wcout<<cnt++<<"\t:\t"<<a_trans_wchar_to_wstr(id)<<endl;

					wcout<<id;
				}
				catch (cls_exception &exc)
				{
					;
				}

				cnt++;
			}

		}
		catch (cls_exception &exc)
		{
			cout<<exc.display()<<endl;
		}

	}
};
