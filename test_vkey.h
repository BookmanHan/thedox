#include "lib_import.h"

while(true)
{
	int n_choose = 0;
	cout<<"\n1.train.\n2.recongnition.\n3.retrain.\n\tchoose:";
	cin>>n_choose;

	appc_vkey_more vkey;
	switch(n_choose)
	{
	case 1:
		{
			vkey.make_training_set("D:\\trys\\vkey\\train\\",15);

			cout<<"\ttimes:";
			int n_times;
			cin>>n_times;

			vkey.train(n_times);

			vkey.save("D:\\trys\\vkey\\train\\lamda.net");
		}
		break;

	case 2:
		{
			vkey.make_training_set("D:\\trys\\vkey\\train\\",15);
			vkey.load("D:\\trys\\vkey\\train\\lamda.net");

			cls_vkey_error verror;

			ifstream fin("D:\\trys\\vkey\\gj_new\\answer.txt");
			ofstream fout("D:\\trys\\vkey\\gj_new\\answer_vkey.txt");

			for(int cnt=0;cnt<500;cnt++)
			{
				string str_answer;
				getline(fin,str_answer);

				cls_image_ot<unsigned> img;
				a_load(img,string("D:\\trys\\vkey\\gj_new\\") + cnt + ".bmp");
				//cout<<cnt<<":\t";
				//cout<<vkey.recongnition(img)<<"\t"<<str_answer<<endl;
				string str_vkey = vkey.recongnition(img);

				fout<<str_vkey<<"\t"<<str_answer<<endl;
				verror.add_sample(str_vkey,str_answer);
				//a_save(img,string("D:\\trys\\vkey\\gj_new\\")+cnt + "_v.bmp");
			}

			verror.display();
			//vkey.update_weights(verror);

			fout.close();
			fin.close();
		}
		break;

	case 3:
		{
			vkey.make_training_set("D:\\trys\\vkey\\train\\",15);

			cout<<"\ttimes:";
			int n_times;
			cin>>n_times;

			vkey.load("D:\\trys\\vkey\\train\\lamda.net");
			vkey.train(n_times);
			vkey.save("D:\\trys\\vkey\\train\\lamda.net");
		}
		break;
	}
}
