#include "lib_import.h"
#include "appc_ann_spilt.h"

#define GETS_VAR(a) {cout<<#a<<":"; cin>>a;}

int spilt_big;
int split_small;
double d_thed;
double d_radius;
double d_limit;
int n_times;
double d_thed_getsin;
double d_lighten;

#define USING_COSINE

struct tag_pos
{
	int n;
	int out_i;
	int out_j;
	int in_i;
	int in_j;

	bool operator<(const tag_pos& tp) const
	{
		return in_i<tp.in_i || in_j < tp.in_j;
	}
};

tag_pos get_index(int n)
{
	tag_pos pos;

	int total_inner = (spilt_big-split_small)*(spilt_big-split_small);
	int big = n/total_inner;
	int dmall = n%total_inner;

	pos.out_i = total_inner/spilt_big;
	pos.out_j = big%spilt_big;
	pos.in_i = dmall/(spilt_big-split_small);
	pos.in_j = dmall%(spilt_big-split_small);
	pos.n = n;
	return pos;
}

int main(int argc, char* argv[])
{
	appc_classifier_word_segment aas(L"D:\\trys\\bpnn_spilt\\t.txt");
	cout<<"-WORDS.READY-"<<endl;
	aas.make_status();
	cout<<"-STATUES.READY-"<<endl;

	while(true)
	{
		cout<<"1.TRAIN."<<endl;
		cout<<"2.SEGMENT."<<endl;
		cout<<"3.SAVE."<<endl;
		cout<<"4.LOAD."<<endl;

		int choose;
		GETS_VAR(choose);

		switch(choose)
		{
		case 1:
			aas.train_network();
			cout<<"-NETWORK.READY-"<<endl;
			break;

		case 2:
			{
				vector<wstring::iterator> vs;
				wstring wstr;
				wcin.imbue(locale("chs"));
				wcin>>wstr;

				{
					aas.word_segment(wstr,vs);

					vector<wstring::iterator>::iterator i_vs = vs.begin();
					wcout.imbue(locale("chs"));

					for(wstring::iterator i=wstr.begin();i<wstr.end();i++)
					{
						wcout<<get_wstr_from_type(*i);
						if (i == *i_vs)
						{
							cout<<'|';
							i_vs++;
						}
					}

					cout<<endl;
				}

				//{
				//	aas.word_segment_commmon(wstr,vs);

				//	vector<wstring::iterator>::iterator i_vs = vs.begin();
				//	wcout.imbue(locale("chs"));

				//	for(wstring::iterator i=wstr.begin();i<wstr.end();i++)
				//	{
				//		wcout<<get_wstr_from_type(*i);
				//		if (i == *i_vs)
				//		{
				//			cout<<'|';
				//			i_vs++;
				//		}
				//	}

				//	cout<<endl;
				//}
			}
			break;

		case 3:
			{
				aas.save_net("a.net");
			}
			break;

		case 4:
			{
				aas.load_net("a.net");
			}
		}
	}

	return 0;

//	while(true)
//	try
//	{
//		string filename;
//		cls_vision vision;
//
//		GETS_VAR(filename);
//		GETS_VAR(d_thed);
//		GETS_VAR(spilt_big);
//		GETS_VAR(split_small);
//		GETS_VAR(d_radius);
//		GETS_VAR(d_thed_getsin);
//		GETS_VAR(d_lighten);
//		GETS_VAR(d_limit);
//		GETS_VAR(n_times);
//
//		filename = "D:\\trys\\" + filename;
//		a_load_image(filename,vision);
//
//		cls_vision v = vision;
//		v.enhance_sequence_rectangle(3,3,-2);
//		a_save_image("D:\\trys\\goodboy.bmp",v);
//		system("D:\\trys\\goodboy.bmp");
//
//		int w = vision.get_width();
//		int h = vision.get_height();
//
//		//vision.geometry_zoom(cls_rect(0,0,512,512));
//
//		cls_vision vision_re = vision;
//		
//		int count_ticked = 1;
//		while(count_ticked--)
//		{
//			for(int color=1;color<4;color++)
//			{
//				cls_cluster_k_means<cls_conject_linear_euclid> cluster(d_thed,split_small*split_small+2,100);
//#ifdef USING_COSINE
//				cls_training_set ts(split_small* split_small + 2,1);
//#else
//				cls_training_set ts(2*split_small* split_small + 2,1);
//#endif 
//				for(int i=0;i<vision.get_width()/spilt_big;i++)
//				{
//					for(int j=0;j<vision.get_height()/spilt_big;j++)
//					{
//						cls_vision v;
//						vision.geometry_part(cls_rect(j*spilt_big,i*spilt_big,(j+1)*spilt_big,(i+1)*spilt_big),v);
//
//#ifdef USING_COSINE
//						cls_matrix<double> mat;
//						v.cosine_transform(mat,color);
//						vector<double> tsd(split_small*split_small + 2),tsc(1);
//#else
//						cls_matrix<complex<double>> mat;
//						v.fourier_transform(mat,color);
//						vector<double> tsd(2*split_small*split_small + 2),tsc(1);
//#endif 
//
//						for(int i=0;i<spilt_big-split_small;i++)
//						{
//							for(int j=0;j<spilt_big-split_small;j++)
//							{
//								for(int ii=0;ii<split_small;ii++)
//								{
//									for(int jj=0;jj<split_small;jj++)
//									{
//#ifdef USING_COSINE
//										tsd[(ii*split_small + jj)] = mat(i+ii,j+jj);
//#else
//										tsd[2*(ii*split_small + jj)] = mat(i+ii,j+jj).real();
//										tsd[2*(ii*split_small + jj) + 1] = mat(i+ii,j+jj).imag();
//#endif
//									}
//								}
//#ifdef USING_COSINE
//								tsd[split_small* split_small] = i;
//								tsd[split_small* split_small + 1] = j;
//#else
//								tsd[2*split_small* split_small] = i;
//								tsd[2*split_small* split_small + 1] = j;
//#endif
// 								ts.add_io_pair(tsd,tsc);
//							}
//						}
//					}
//
//				}
//
//				cout<<"C"<<endl;
//				if (ts.get_size() == 0)
//					continue;
//
//				map<int,pair<map<tag_pos,int>,int>> classes;
//				vector<int> res = cluster.train_example(ts);
//				for(int cnt=0;cnt<res.size();cnt++)
//				{
//					if (classes.find(res[cnt]) != classes.end())
//					{
//						classes[res[cnt]].first[get_index(cnt)]++;
//						classes[res[cnt]].second++;
//					}
//					else
//					{
//						map<tag_pos,int> stp;
//						stp[get_index(cnt)] = 1;
//						classes[res[cnt]] = make_pair(stp,1);
//					}
//				}
//
//#ifdef USING_COSINE
//				cls_matrix<double> mat_vision;
//				vision.cosine_transform(mat_vision,color);
//#else
//				cls_matrix<complex<double>> mat_vision;
//				vision.fourier_transform(mat_vision,color);
//#endif
//
//				map<int,pair<map<tag_pos,int>,int>>::iterator itor,itor_memd;
//				vector<pair<double,map<int,pair<map<tag_pos,int>,int>>::iterator>> vitor;
//
//				for(itor=classes.begin();itor != classes.end();itor++)
//				{
//					pair<double,map<int,pair<map<tag_pos,int>,int>>::iterator> pin;
//					pin.first = fabs( double(itor->second.second) / double(itor->second.first.size()) - 1.0);
//					pin.second = itor;
//
//					vitor.push_back(pin);
//				}
//
//				for(int cnt=0;cnt<n_times;cnt++)
//				{
//					int cnt_big = 0;
//					for(int cnt=0;cnt<vitor.size();cnt++)
//					{
//						if (vitor[cnt].first < vitor[cnt_big].first)
//						{
//							cnt_big = cnt;
//						}
//					}
//
//					vitor[cnt_big].first = 1000000;
//					itor_memd = vitor[cnt_big].second;
//
//					cls_matrix<double> matb(spilt_big,spilt_big);
//					matb.fill(100000);
//
//					map<tag_pos,int>::iterator itor_set,itor_set_mem;
//					for(itor_set=itor_memd->second.first.begin();itor_set != itor_memd->second.first.end();itor_set++)
//					{
//						if (itor_set->second > d_radius * double(itor_memd->second.second) / double(itor_memd->second.first.size()))
//						{
//							for(int i=0;i<spilt_big;i++)
//							{
//								for(int j=0;j<spilt_big;j++)
//								{
//									if (itor_set->first.in_i + i >= spilt_big || itor_set->first.in_j + j >= spilt_big)
//										continue;
//
//									matb(itor_set->first.in_i + i,itor_set->first.in_j + j)--;
//								}
//							}
//						}
//					}
//
//					double max_times = matb(0,0);
//					for(int i=0;i<spilt_big;i++)
//					{
//						for(int j=0;j<spilt_big;j++)
//						{
//							max_times = min(max_times,matb(i,j));
//						}
//					}
//
//					//cout<<max_times<<endl;
//					for(int i=0;i<spilt_big;i++)
//					{
//						for(int j=0;j<spilt_big;j++)
//						{
//							matb(i,j) -= max_times;
//							max_times = (100000.0 - max_times);
//
//							if (max_times < e)
//								matb(i,j) = 1;
//							else
//								matb(i,j) /= max_times;
//						}
//					}
//
//					cls_matrix<double> matbig(scls_signal_trans::get_fourier_size(vision.get_width()),scls_signal_trans::get_fourier_size(vision.get_height()));
//					
//					double d_x = double(matb.get_width())/double(matbig.get_width());
//					double d_y = double(matb.get_height())/double(matbig.get_height());
//
//					for(int i=0;i<matbig.get_width();i++)
//					{
//						for(int j=0;j<matbig.get_height();j++)
//						{
//							double posi = i * d_x + 0.5;
//							double posj = j * d_y + 0.5;
//							posi = min(posi,matb.get_width()-2);
//							posj = min(posj,matb.get_height()-2);
//
//							int a = (int)posi;
//							int b = (int)posj;
//
//							double r1 = (a+1 - posi)*matb(a,b)*(b+1-posj) + matb(a+1,b) * (posi-a)*(b+1-posj)
//								+ matb(a,b+1) * (a + 1 -posi) * (posj - b) + matb(a+1,b+1) * (posi-a)*(posj-b);
//
//							matbig(i,j) = r1;
//						}
//					}
//
//					//cout<<"D"<<endl				
//					for(int i=0;i<matbig.get_width();i++)
//					{
//						for(int j=0;j<matbig.get_height();j++)
//						{
//#ifdef USING_COSINE
//							if (matbig(i,j) < d_thed_getsin)
//								mat_vision(i,j) = 0;
//							else
//								mat_vision(i,j) *= d_lighten * matbig(i,j);
//#else
//							mat_vision(i,j) = complex<double>(0,0);
//#endif
//						}
//					}
//
//#ifdef USING_COSINE
//					vision_re.i_cosine_transform(mat_vision,color);
//#else
//					vision_re.i_fourier_transform(mat_vision,color);
//#endif
//				}
//			}
//
//			cout<<"F"<<endl;
//			a_save_image(string("D:\\trys\\cluiso") + count_ticked + ".bmp",vision_re);
//			system((string("D:\\trys\\cluiso") + count_ticked + ".bmp").c_str());
//		}
//	}
//	catch (cls_exception& e)
//	{
//		cout<<"E"<<e.display()<<endl;
//	}
//	catch(exception& ex)
//	{
//		cout<<"E"<<ex.what()<<endl;
//	}
//
//	return 0;
//
	//try
	//{
	//	cls_vision vision;
	//	a_load_image("D:\\trys\\iso.bmp",vision);

	//	GETS_VAR(d_thed);
	//	GETS_VAR(d_r);
	//	GETS_VAR(spilt_big);
	//	GETS_VAR(split_small);

	//	cls_vision vision_re = vision;
	//	for(int cnt=1;cnt<4;cnt++)
	//	{
	//		cls_cluster_max_min_distance<cls_conject_linear_euclid> cluster(d_thed);
	//		cls_training_set ts(2 * split_small* split_small,0);
	//
	//		for(int i=0;i<vision.get_width()/spilt_big;i++)
	//		{
	//			for(int j=0;j<vision.get_height()/spilt_big;j++)
	//			{
	//				cls_vision v;
	//				vision.geometry_part(cls_rect(j*spilt_big,i*spilt_big,(j+1)*spilt_big,(i+1)*spilt_big),v);

	//				cls_matrix<complex<double> > mat;
	//				v.fourier_transform(mat,cnt);

	//				vector<double> tsd(2*split_small*split_small),tsc(1);
	//				for(int i=0;i<spilt_big;i+=split_small)
	//				{
	//					for(int j=0;j<spilt_big;j+=split_small)
	//					{
	//						if (i*i + j*j < d_r)
	//						{
	//							continue;
	//						}

	//						for(int ii=0;ii<split_small;ii++)
	//						{
	//							for(int jj=0;jj<split_small;jj++)
	//							{
	//								tsd[(2*(ii*split_small + jj))] = mat(i+ii,j+jj).real();
	//								tsd[(2*(ii*split_small + jj)) + 1] = mat(i+ii,j+jj).imag();
	//							}
	//						}

	//						ts.add_io_pair(tsd,tsc);
	//					}
	//				}
	//			}
	//		}

	//		cout<<"C"<<endl;
	//		map<int,pair<map<tag_pos,int>,int>> classes;
	//		vector<int> res = cluster.train_example(ts);
	//		for(int cnt=0;cnt<res.size();cnt++)
	//		{
	//			if (classes.find(res[cnt]) != classes.end())
	//			{
	//				classes[res[cnt]].first.insert(get_index(cnt));
	//				classes[res[cnt]].second++;
	//			}
	//			else
	//			{
	//				map<tag_pos,int> stp;
	//				stp[get_index(cnt)] = 1;

	//				classes[res[cnt]].first.insert(get_index(cnt));
	//				classes[res[cnt]].second++;
	//			}
	//		}

	//		double d_cedu = 1000000;
	//		map<int,pair<map<tag_pos,int>,int>>::iterator itor,itor_memd;
	//		for(itor=classes.begin();itor!=classes.end();itor++)
	//		{
	//			if (fabs(itor->second.first.size()/double(itor->second.second)-1.0 ) < d_cedu)
	//			{
	//				d_cedu = fabs(itor->second.first.size()/double(itor->second.second) - 1.0);
	//				itor_memd = itor;
	//			}
	//		}

	//		cout<<d_cedu<<endl;

	//		cls_matrix<bool> matb(spilt_big,spilt_big);
	//		matb.fill(false);
	//		set<tag_pos>::iterator itor_set;
	//		for(itor_set=itor_memd->second.first.begin();itor_set != itor_memd->second.first.end();itor_set++)
	//		{
	//				for(int i=0;i<split_small;i++)
	//				{
	//					for(int j=0;j<split_small;j++)
	//					{
	//						matb(itor_set->in_i + i,itor_set->in_j + j) = true;
	//					}
	//				}
	//		}

	//		//cls_vision vi_tmp;
	//		//vi_tmp.mat_image = matb;
	//		//cls_matrix<complex<double>> mat;
	//		//vision.fourier_transform(mat,cnt);
	//		//vi_tmp.geometry_zoom(cls_rect(0,0,mat.get_height(),mat.get_width()));
	//		//for(int i=0;i<mat.get_width();i++)
	//		//{
	//		//	for(int j=0;j<mat.get_height();j++)
	//		//	{
	//		//		if (vi_tmp.get_gray(i,j) < 127)
	//		//			mat(i,j)  = complex<double>(0,0);//complex<double>(sqrt(mat(i,j).real()*mat(i,j).real()+mat(i,j).imag()*mat(i,j).imag()),0);
	//		//	}
	//		//}

	//		//vision_re.i_fourier_transform(mat,cnt);

	//		for(int i=0;i<vision.get_width()/spilt_big;i++)
	//		{
	//			for(int j=0;j<vision.get_height()/spilt_big;j++)
	//			{
	//				cls_vision v;
	//				vision.geometry_part(cls_rect(j*spilt_big,i*spilt_big,(j+1)*spilt_big,(i+1)*spilt_big),v);

	//				cls_matrix<complex<double> > mat;

	//				v.fourier_transform(mat,cnt);

	//				for(int i=0;i<mat.get_width();i++)
	//				{
	//					for(int j=0;j<mat.get_height();j++)
	//					{
	//						if (i*i + j*j < d_r)
	//						{
	//							continue;
	//						}

	//						if (matb(i,j) == false)
	//							mat(i,j) = complex<double>(0,0);//complex<double>(0,0);
	//					}
	//				}

	//				v.i_fourier_transform(mat,cnt);
	//				vision_re.geometry_locate(v,i*spilt_big,j*spilt_big,cnt);
	//			}
	//		}
	//	}
	//}
	//catch (cls_exception& e)
	//{
	//	cout<<"E"<<e.display()<<endl;
	//}
	//catch(exception& ex)
	//{
	//	cout<<"E"<<ex.what()<<endl;
	//}

	//return 0;
}