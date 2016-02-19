#include "lib_import.h"
#pragma  once

#define FIXED_1 0x00010000
#define FIXED_half 0x00008000
#define LONG_TO_FIXED(l)	((l)*FIXED_1)
#define FLOAT_TO_FIXED(f)	((long)((f)*FIXED_1))
#define FIXED_TO_SHORT(f)	((short)((f)>>16))
#define FIXED_TO_BYTE(f)	((BYTE)((f)>>16))
#define FIXED_TO_LONG(f)	((f)>>16)

#define JPEGCPERR_NOERROR				0
#define JPEGCPERR_UNKNOWNERROR			1
#define JPEGCPERR_UNSUPPORTEDVERSION	2
#define JPEGCPERR_QTERROR				3
#define JPEGCPERR_FRAMEERROR			4
#define JPEGCPERR_HTERROR				5
#define JPEGCPERR_SOSERROR				6

#define IDCT_FLOAT		0
#define IDCT_INT		1
#define IDCT_MMX		2

#define ICHG(c1,c2)		((((UINT)(c1))<<8) + (c2))

#define NEXT_BYTE														\
	{	if(bit_idx == 0)													\
		{	if(cc[buf_index] == 0xff)									\
			{	\
				while(cc[buf_index+1] == 0xff)							\
				{	buf_index++;											\
					remain_buf--;										\
				}																			\
																						\
				if(cc[buf_index+1] != 0)									\
					goto sync;											\
			}															\
			else if(cc[buf_index] == 0 && cc[buf_index-1] == 0xff)		\
			{	\
				buf_index++;												\
				remain_buf--;											\
			\
				if(cc[buf_index] == 0xff)								\
				{	\
					while(cc[buf_index+1] == 0xff)						\
					{	\
						buf_index++;										\
						remain_buf--;									\
					}	\
																		\
					if(cc[buf_index+1] != 0)								\
					goto sync;										\
				}														\
			}															\
		}																\
	}

#define NO_ZERO_ROW_TEST
#define BITS_IN_JSAMPLE 8

#define MAXJSAMPLE	255
#define CENTERJSAMPLE	128

#define CONST_BITS  13
#define PASS1_BITS  2

#define FIX_0_298631336  ((INT32)  2446)	/* FIX(0.298631336) */
#define FIX_0_390180644  ((INT32)  3196)	/* FIX(0.390180644) */
#define FIX_0_541196100  ((INT32)  4433)	/* FIX(0.541196100) */
#define FIX_0_765366865  ((INT32)  6270)	/* FIX(0.765366865) */
#define FIX_0_899976223  ((INT32)  7373)	/* FIX(0.899976223) */
#define FIX_1_175875602  ((INT32)  9633)	/* FIX(1.175875602) */
#define FIX_1_501321110  ((INT32)  12299)	/* FIX(1.501321110) */
#define FIX_1_847759065  ((INT32)  15137)	/* FIX(1.847759065) */
#define FIX_1_961570560  ((INT32)  16069)	/* FIX(1.961570560) */
#define FIX_2_053119869  ((INT32)  16819)	/* FIX(2.053119869) */
#define FIX_2_562915447  ((INT32)  20995)	/* FIX(2.562915447) */
#define FIX_3_072711026  ((INT32)  25172)	/* FIX(3.072711026) */

#define ONE	((INT32) 1)
#define RIGHT_SHIFT(x,shft)  \
	((shift_temp = (x)) < 0 ? \
	(shift_temp >> (shft)) | ((~((INT32) 0)) << (32-(shft))) : \
	(shift_temp >> (shft)))
#define DESCALE(x,n)  RIGHT_SHIFT((x) + (ONE << ((n)-1)), n)
#define RANGE_MASK  (MAXJSAMPLE * 4 + 3) /* 2 bits wider than legal samples */
#define RANGE_LIMIT(i) \
	if((i) > MAXJSAMPLE)	\
	(i) = MAXJSAMPLE;	\
		else if((i) < 0)	\
		(i) = 0

struct tag_jpg_node
{
	tag_jpg_node* left;
	tag_jpg_node* right;
	bool is_leaf;

	unsigned char code;
};

struct tag_jpg_param
{
	unsigned char * stream;
	unsigned int buf_idx;
	unsigned int bit_idx;
	unsigned int remain;
};

class cls_jpg_huffman_table
{
private:
	tag_jpg_node root;

public:
	cls_jpg_huffman_table()
	{
		root.left = null;
		root.right = null;
		root.is_leaf = false;
	}

	~cls_jpg_huffman_table()
	{
		clear();
	}

	bool add_node(unsigned int len,unsigned char c)
	{
		return add_node(&root,len,c);
	}

	bool add_node(tag_jpg_node* node,unsigned len, unsigned char c)
	{
		if (len == 1)
		{
			if (node->left == null)
			{
				node->left = new tag_jpg_node;
				node->left->left = null;
				node->left->right = null;
				node->left->code = c;
				node->left->is_leaf = true;

				return true;
			}

			if (node->right == null)
			{
				node->right = new tag_jpg_node;
				node->right->left = null;
				node->right->right = null;
				node->right->code = c;
				node->right->is_leaf = true;

				return true;
			}

			return false;
		}
		else
		{
			if (node->left == null)
			{
				node->left = new tag_jpg_node;
				node->left->left = null;
				node->left->right = null;
				node->left->is_leaf = false;

				return add_node(node->left,len-1,c);
			}
			if (node->left->is_leaf == false)
			{
				if (add_node(node->left,len-1,c) == true)
					return true;
			}

			if (node->right == null)
			{
				node->right = new tag_jpg_node;
				node->right->left = null;
				node->right->right = null;
				node->right->is_leaf = false;
				
				return add_node(node->right,len-1,c); 
			}

			if (node->right->is_leaf == false)
			{
				return add_node(node->right,len-1,c);
			}

			return false;
		}
	}

	void clear()
	{
		if (root.left != null)
			clear_node(root.left);
		if (root.right != null)
			clear_node(root.right);

		root.left = null;
		root.right = null;
	}

	void clear_node(tag_jpg_node* node)
	{
		if (node->left != null)
			clear_node(node->left);
		if(node->right != null)
			clear_node(node->right);

		delete node;
	}
	
	bool decode(tag_jpg_param* param,unsigned char* p)
	{
		unsigned char uc;
		tag_jpg_node *n = & root;

		unsigned char *stream = param->stream + param->buf_idx;

		unsigned buf_idx,buf_idx_bk;
		buf_idx = buf_idx_bk = param->buf_idx;

		unsigned bit_idx = param->bit_idx;
		uc = (*stream++);
		buf_idx++;

		if (uc == 0xff)
		{
			while((*stream) == 0xff)
			{
				stream++;
				buf_idx++;
			}

			if ((*stream) != 0)
				return false;
			else
				stream++,buf_idx++;
		}

		uc <<= bit_idx;

		while(true)
		{
			if (uc & 0x80)
			{
				n = n->right;
			}
			else
			{
				n = n->left;
			}

			bit_idx ++;

			if (n == null)
			{
				n = &root;
				bit_idx = 8;
				goto ttt;
			}

			if(n->is_leaf == true)
			{
				param->bit_idx = bit_idx;
				if (bit_idx == 8)
				{
					param->buf_idx = buf_idx;
					param->bit_idx = 0;
				}

				param->remain -= param->buf_idx - buf_idx_bk;
				*p = n->code;

				return true;
			}
ttt:
			if (bit_idx == 8)
			{
				bit_idx = 0;
				param->buf_idx = buf_idx;
				uc = * stream ++;
				buf_idx++;

				if (uc == 0xff)
				{
					while((*stream) == 0xff)
					{
						stream++;
						buf_idx++;
					}

					if ((*stream) != 0)
					{
						param->remain -= param->buf_idx - buf_idx_bk;
						return false;
					}
					else
					{
						stream++;
						buf_idx++;
					}
				}

				continue;
			}
			uc <<= 1;
		}

		param->remain -= param->buf_idx - buf_idx_bk;
		return false;
	}

public:
	friend class cls_format_jpg;
};

struct tag_jpg_componet
{
	unsigned int v;
	unsigned int h;
	unsigned int qt_id;
};

struct tag_jpg_info
{
	unsigned int width;
	unsigned int height;
	unsigned int buf_width;
	unsigned int buf_height;

	int com_num;

	unsigned char* image[5];
};

class cls_format_jpg
{
protected:
	unsigned int width;
	unsigned int height;
	unsigned int buf_width;
	unsigned int buf_height;
	unsigned int com_num;

	unsigned int ri;
	unsigned int qt[4][64];

	cls_jpg_huffman_table ht_dc[4];
	cls_jpg_huffman_table ht_ac[4];

	tag_jpg_componet com[6];

	unsigned int max_h;
	unsigned int max_v;
	unsigned int num_unit_h;
	unsigned int num_unit_v;

	void (cls_format_jpg::*func_ldct)(short*,byte*);

public:
	byte * image[5];

protected:
	void init()
	{
		for(int cnt=0;cnt<5;cnt++)
		{
			image[cnt] = null;
		}

		max_h = 0;
		max_v = 0;

		num_unit_h = 0;
		num_unit_v = 0;
	}

	void store_block(unsigned com_id,unsigned b_num,byte* block,byte* img)
	{
		unsigned x,y,ux,uy,bx,by,u_num,mh,mv,j,k,m,n;
		byte *b,*p,*p_img = img;

		u_num = b_num / (com[com_id].h * com[com_id].v);
		b_num = b_num % (com[com_id].h * com[com_id].v);

		ux = u_num % num_unit_h;
		uy = u_num / num_unit_h;
		bx = b_num % com[com_id].h;
		by = b_num / com[com_id].h;

		if (ux >= num_unit_h || uy >= num_unit_v)
			return;

		ux *= max_h*8;
		uy *= max_v*8;

		mh = max_h / com[com_id].h;
		mv = max_v / com[com_id].v;

		bx *= mh *8;
		by *= mv *8;

		x = ux + bx;
		y = uy + by;

		p_img += buf_width * y + x;

		for(m = 0;m<mv;m++)
		{
			for(n=0;n<mh;n++)
			{
				b = block;
				for(j=0;j<8;j++)
				{
					p = p_img + (m + j*mv)*buf_width + n;
					for(k=0;k<8;k++)
					{
						*p = *b ++;
						p += mh;
					}
				}
			}
		}
	}

	#pragma warning( disable : 4035 )
	inline long fixedMul(long a,long b)
	{	__asm
	{	mov		eax,a
	mov		ebx,b
	imul	ebx
	shrd	eax,edx,16
	}
	}


	void color_space_trans()
	{
		unsigned j,k;
		byte *p1,*p2,*p3;

		long a,b,c,d,y,cb,cr,l,rr,gg,bb;
		if (com_num != 3)
			return;

		p1 = image[0];
		p2 = image[1];
		p3 = image[2];

		a = FLOAT_TO_FIXED((float)1.402);
		b = FLOAT_TO_FIXED((float)0.34414);
		c = FLOAT_TO_FIXED((float)0.71414);
		d = FLOAT_TO_FIXED((float)1.772);
		l = LONG_TO_FIXED(128);

		for(j=0;j<buf_height;j++)
		{
			for(k=0;k<buf_width;k++)
			{
				y = LONG_TO_FIXED((long)(*p1));
				cb = LONG_TO_FIXED((long)(*p2)) - l;
				cr = LONG_TO_FIXED((long)(*p3)) - l;
				rr = FIXED_TO_LONG(y+fixedMul(a,cr));
				gg = FIXED_TO_LONG(y-fixedMul(b,cb)-fixedMul(c,cr));
				bb = FIXED_TO_LONG(y+fixedMul(d,cb));

				if(rr>255)
					rr = 255;
				else if(rr<0)
					rr = 0;
				if(gg>255)
					gg = 255;
				else if(gg<0)
					gg = 0;
				if(bb>255)
					bb = 255;
				else if(bb<0)
					bb = 0;
				*p1 = (BYTE)rr;
				*p2 = (BYTE)gg;
				*p3 = (BYTE)bb;
				
				p1++,p2++,p3++;
			}
		}
	}

	bool read_q_table(ifstream & fin)
	{
		unsigned char c;
		unsigned char len[2];
		int length;
		unsigned int jd,qt_id;

		fin.read((char*)len,2);

		length = (int)(ICHG(len[0],len[1])-2);
		while(length > 0)
		{
			fin.read((char*)&c,1);
			--length;

			qt_id = c & 0x0f , jd = c >> 4;
			if (jd != 0 || qt_id >= 4)
			{
				return false;
			}

			for(int cnt=0;cnt<64;cnt++)
			{
				fin.read((char*)&c,1);
				qt[qt_id][cnt] = c;
			}

			length -= 64;
		}

		return true;
	}

	bool read_h_table(ifstream & fin)
	{
		unsigned char c;
		unsigned char len[2];
		unsigned char cn[16];

		int length;

		unsigned int ht_id;
		unsigned int type;

		cls_jpg_huffman_table *h;
		fin.read((char*)len,2);

		length = (int)(ICHG(len[0],len[1])-2);

		while(length > 0)
		{
			fin.read((char*)&c,1);
			length--;

			ht_id = c & 0x0f ,type = (c >> 4) & 0x01;

			if (ht_id >= 2)
				return false;

			if (type == 0)
				h = & ht_dc[ht_id];
			else
				h = &ht_ac[ht_id];

			fin.read((char*)cn,16);

			length -= 16;

			for(int i=0;i<16;i++)
			{
				for(int j=0;j<cn[i];j++)
				{
					fin.read((char*)&c,1);
					length--;
					h->add_node(i+1,c);
				}
			}
		}

		return true;
	}

	bool read_sof_0(ifstream & fin)
	{
		unsigned char c;
		unsigned char len[2];
		int length;

		fin.read((char*)len,2);
		length = (int)(ICHG(len[0],len[1])-2);

		fin.read((char*)&c,1);

		if (c != 8)
			return false;

		fin.read((char*)len,2);
		height = ICHG(len[0],len[1]);
		fin.read((char*)len,2);
		width = ICHG(len[0],len[1]);

		if (height == 0 || height == 0)
			return false;

		fin.read((char*)&c,1);
		com_num = c;

		for(int cnt=0;cnt<com_num;cnt++)
		{
			fin.read((char*)&c,1);
			if (c > 5)
				return false;

			fin.read((char*)len,2);
			com[c].h = len[0] >> 4 , com[c].v = len[0] & 0x0f;
			com[c].qt_id = len[1];

			if (com[c].h == 0 || com[c].v == 0)
				return false;

			if (max_h <  com[c].h)
				max_h = com[c].h;
			if (max_v < com[c].v)
				max_v = com[c].v;
		}

		num_unit_h = (width + max_h*8 - 1) / (max_h * 8);
		num_unit_v = (height + max_v*8 - 1)/ (max_v * 8);

		buf_width = num_unit_h * max_h * 8;
		buf_height = num_unit_v * max_v * 8;

		for(int cnt=0;cnt<com_num;cnt++)
		{
			image[cnt] = new byte[buf_height * buf_width];
		}

		return true;
	}

	bool read_ri(ifstream & fin)
	{
		unsigned char len[2];
		unsigned length;

		fin.read((char*)len,2);
		length = ICHG(len[0],len[1]);

		if (length != 4)
			return false;

		fin.read((char*)len,2);
		ri = ICHG(len[0],len[1]);

		return true;
	}

	bool read_sos(ifstream & fin)
	{
		unsigned char c;
		unsigned char len[2];

		unsigned length;
		unsigned com_num;
		unsigned com_id[4];
		unsigned ht_dc_id[4];
		unsigned ht_ac_id[4];
		unsigned block_idx[4];

		fin.read((char*)len,2);
		length = (int)(ICHG(len[0],len[1])-2);

		fin.read((char*)&c,1);

		if (c < 1 || c > 4)
			return false;

		com_num = c;

		for(int cnt=0;cnt<com_num;cnt++)
		{
			fin.read((char*)&c,1);
			if (c > 5)
				return false;

			com_id[cnt] = c;
			fin.read((char*)&c,1);

			ht_dc_id[cnt] = c >> 4;
			ht_ac_id[cnt] = c & 0x0f;

			if (ht_dc_id[cnt] > 4 || ht_ac_id[cnt] > 4)
			{
				return false;
			}
		}

		fin.seekg(3,ios::cur);

		unsigned dc_block[4];
		unsigned dc_ac[100];
		short dcac_uz[64];
		byte image_block[64];
		unsigned char cc[140];
		int remain_buf = 0;
		int bit_idx = 0;
		unsigned buf_index = 0;
		bool read_ok = false;
		unsigned char cv;
		tag_jpg_param param;
		bool ret_val;
		
		block_idx[0] = 0;
		block_idx[1] = 0;
		block_idx[2] = 0;
		block_idx[3] = 0;

		dc_block[0] = 0;
		dc_block[1] = 0;
		dc_block[2] = 0;
		dc_block[3] = 0;

		param.stream = cc;

		int l;

start_scan:
		while(true)
		{
			for(int i=0;i<com_num;i++)
			{
				cls_jpg_huffman_table *ht_a,*ht_d;

				ht_d = ht_dc + ht_ac_id[i];
				ht_a = ht_ac + ht_ac_id[i];
				unsigned id = com_id[i];

				for(int j=0;j<com[id].h * com[id].v;j++)
				{
					for(l=0;l<64;)
					{
						if (remain_buf <= 30 && read_ok == false)
						{
							for(int k=0;k<remain_buf;k++)
								cc[k] = cc[buf_index + k];

							buf_index = 0;

							fin.read((char*)cc + remain_buf, 100);
							remain_buf += fin.gcount();

							if (fin.bad())
								return false;

							if (fin.eof())
							{
								read_ok = true;
								fin.clear();
							}
						}

						param.buf_idx = buf_index;
						param.bit_idx = bit_idx;
						param.remain = remain_buf;

						if (l == 0)
						{
							ret_val = ht_d->decode(&param,&cv);
						}
						else
						{
							ret_val = ht_a->decode(&param,&cv);
						}

						bit_idx = param.bit_idx;
						buf_index = param.buf_idx;
						remain_buf = param.remain;
						bit_idx %= 8;

						if (remain_buf <= 0)
							return false;

						if (ret_val == true)
						{
							unsigned int u,zn,gn,bno;
							unsigned char ct[4];
							int bn,bn1,it;

							if (l == 0)
							{
								if (cv == 0)
								{
									u = 0;
								}
								else
								{
									bn = (int)cv;
									it = 0;
									bno = bit_idx;

									NEXT_BYTE;

									while(bn > 0 && it < 4)
									{
										bn1 = 8 - bit_idx;
										ct[it] = cc[buf_index];

										if (bn >= bn1)
										{
											buf_index++;
											bit_idx = 0;
											remain_buf--;
										}
										else
										{
											bit_idx += bn;
										}

										bn -= bn1;
										it ++;

										NEXT_BYTE;
									}

									u = (*((unsigned int*)ct));
									
									_asm mov eax,u
									_asm bswap eax
									_asm mov u,eax

									u <<= bno;

									if (((int)u) > 0)
									{
										u = 0 - ((~u)>>(32 - cv));
									}
									else
									{
										u = u >> (32 - cv);
									}
								}

								dc_ac[0] = dc_block[i] = dc_block[i] + u;
								l++;
 							}
							else
							{
								if (cv == 0)
								{
									while (l < 64)
									{
										dc_ac[l] = 0;
										l++;
									}
								}
								else
								{
									unsigned int m;
									if (cv == 0xf0)
									{
										for(m =0 ;m<16;m++)
										{
											dc_ac[l] = 0;
											l++;
										}
									}
									else
									{
										zn = cv >> 4;
										gn = cv & 0x0f;

										for(m=0;m<zn;m++)
										{
											dc_ac[l] = 0;
											l++;
										}

										if (gn != 0)
										{
											bn = (int)gn;
											it = 0;
											bno = bit_idx;

											NEXT_BYTE;

											while(bn > 0 && it < 4)
											{
												bn1 = 8 - bit_idx;
												ct[it] = cc[buf_index];

												if (bn >= bn1)
												{
													buf_index++;
													bit_idx = 0;
													remain_buf--;
												}
												else
												{
													bit_idx += bn;
												}

												bn -= bn1;
												it++;
												NEXT_BYTE;
											}

											u = (*((UINT*)ct));
											_asm mov	eax,u
											_asm bswap	eax
											_asm mov	u,eax

											u <<= bno;
											
											if(((INT)u) > 0)
											{
												u = 0 - ((~u)>>(32 - gn));
											}
											else
											{
												u = u >> (32 - gn);
											}

											dc_ac[l] = u;
											l++;
										}
									}
								}
							}
						}
						else
							goto sync;
					}

					if (l > 64)
						goto sync;

					unsigned *q = qt[com[id].qt_id];
					for(int k=0;k<64;k++)
					{
						dc_ac[k] =  ((int)dc_ac[k])*((int)q[k]);
					}

					for(int k=0;k<64;k++)
					{
						dcac_uz[format_jpg_zigzag()[k]] = (short)((int)dc_ac[k]);
					}

					byte* b = image_block;
					(this->*func_ldct)(dcac_uz,b);
					store_block(id,block_idx[i],b,image[i]);
					block_idx[i]++;
				}
			}
		}

sync:
		while(true)
		{
			unsigned char c1,c2;
			c1 = cc[buf_index];
			c2 = cc[buf_index + 1];

			if (c1 == 0xff && c2 != 0xff && c2 != 0)
			{
				if (c2 <0xd0 || c2 > 0xd7)
				{
					fin.seekg(-remain_buf,ios::cur);
					return true;
				}
				else
				{
					dc_block[0] = 0;
					dc_block[1] = 0;
					dc_block[2] = 0;
					dc_block[3] = 0;

					buf_index += 2;
					bit_idx = 0;
					remain_buf = 2;

					break;
				}
			}
			else
			{
				if (remain_buf <= 30 && read_ok == false)
				{
					for(int k=0;k<remain_buf;k++)
					{
						cc[k] = cc[buf_index + k];
					}

					buf_index = 0;
					fin.read((char*)cc + remain_buf,100);
					remain_buf += fin.gcount();

					if (fin.bad())
						return false;
					
					if (fin.eof())
					{
						read_ok = true;
						fin.clear();
					}
				}

				buf_index++;
				remain_buf--;

				if (remain_buf <= 0)
					return true;

				c1 = cc[buf_index];
				c2 = cc[buf_index+1];
			}
		}

		goto start_scan;
	}

public:
	cls_format_jpg()
	{
		init();
	}

	cls_format_jpg(string str_filename,unsigned idct_mode)
	{
		init();
		open(str_filename,idct_mode);
	}

	~cls_format_jpg()
	{
		close();
	}

	void open(string str_filename,unsigned idct_mode)
	{
		unsigned char seg[2];
		unsigned char len[2];

		init();

		switch(idct_mode)
		{
		case IDCT_FLOAT:
			func_ldct = &cls_format_jpg::idct_float;
			break;
		case IDCT_INT:
			func_ldct = &cls_format_jpg::idct_int;
			break;
		}

		ifstream fin(str_filename.c_str(),ios::binary);
		if (!fin)
		{
			throw cls_exception_io("file is bad for input.");
		}

		fin.read((char*)seg,2);
		if (seg[0] != 0xff || seg[1] != 0xd8)
		{
			throw cls_exception_not_support("bad format");
		}

		while(true)
		{
			fin.read((char*)seg,2);
			if (seg[0] != 0xff)
				throw cls_exception_not_support("bad format");

			while(seg[1] == 0xff)
			{
				fin.read((char*)seg+1,1);
			}

			switch(seg[1])
			{
			case 0x01:
			case 0xd0:
			case 0xd1:
			case 0xd2:
			case 0xd3:
			case 0xd4:
			case 0xd5:
			case 0xd6:
			case 0xd7:
				break;

			case 0xc1:
			case 0xc2:
			case 0xc3:
			case 0xc5:
			case 0xc6:
			case 0xc7:
			case 0xc8:
			case 0xc9:
			case 0xca:
			case 0xcb:
			case 0xcc:
			case 0xcd:
			case 0xce:
			case 0xcf:
				throw cls_exception_not_support("bad format");
				break;

			case 0xdb:
				if (read_q_table(fin) == false)
				{
					throw cls_exception_not_support("bad format");
				}
				break;

			case 0xc0:
				if (read_sof_0(fin) == false)
				{
					throw cls_exception_not_support("bad format");
				}
				break;

			case 0xdd:
				if (read_ri(fin) == false)
				{
					throw cls_exception_not_support("bad format");
				}
				break;

			case 0xc4:
				if (read_h_table(fin) == false)
				{
					throw cls_exception_not_support("bad format");
				}
				break;

			case 0xda:
				if (read_sos(fin) == false)
				{
					throw cls_exception_not_support("bad format");
				}
				break;

			case 0xd9:
				color_space_trans();
				return;
				break;

			default:
				fin.read((char*)len,2);
				fin.seekg(ICHG(len[0],len[1])-2,ios::cur);
				break;
			}
		}
	}

	void get_image_info(tag_jpg_info* info)
	{
		info->buf_width = buf_width;
		info->buf_height = buf_height;
		info->width = width;
		info->height = height;
		info->com_num = com_num;
		info->image[0] = image[0];
		info->image[1] = image[1];
		info->image[2] = image[2];
		info->image[3] = image[3];
		info->image[4] = image[4];
	}

	void close()
	{
		for(int i=0;i<4;i++)
		{
			ht_dc[i].clear();
			ht_ac[i].clear();
		}

		for(int i=0;i<5;i++)
		{
			if (image[i] != null)
			{
				delete [] image[i];
				image[i] = null;
			}
		}
	}

	void idct_int(short* coef_block,BYTE *output_buf)
	{	
		INT32 tmp0, tmp1, tmp2, tmp3;
		INT32 tmp10, tmp11, tmp12, tmp13;
		INT32 z1, z2, z3, z4, z5;
		int itp;
		short *inptr;
		int * wsptr;
		char *outptr;
		int ctr;
		int workspace[64];
		INT32 shift_temp;

		inptr = coef_block;
		wsptr = workspace;
		for (ctr = 8; ctr > 0; ctr--)
		{
			if ((inptr[8] | inptr[16] | inptr[24] | inptr[32] | inptr[40] | inptr[48] |
		inptr[56]) == 0) 
		{	int dcval = inptr[0] << PASS1_BITS;
		wsptr[0] = dcval;
		wsptr[8] = dcval;
		wsptr[16] = dcval;
		wsptr[24] = dcval;
		wsptr[32] = dcval;
		wsptr[40] = dcval;
		wsptr[48] = dcval;
		wsptr[56] = dcval;
		inptr++;
		wsptr++;
		continue;
		}
		z2 = inptr[16];
		z3 = inptr[48];
		z1 = (z2 + z3)*FIX_0_541196100;
		tmp2 = z1 + z3*(- FIX_1_847759065);
		tmp3 = z1 + z2*FIX_0_765366865;
		z2 = inptr[0];
		z3 = inptr[32];
		tmp0 = (z2 + z3) << CONST_BITS;
		tmp1 = (z2 - z3) << CONST_BITS;
		tmp10 = tmp0 + tmp3;
		tmp13 = tmp0 - tmp3;
		tmp11 = tmp1 + tmp2;
		tmp12 = tmp1 - tmp2;
		tmp0 = inptr[56];
		tmp1 = inptr[40];
		tmp2 = inptr[24];
		tmp3 = inptr[8];
		z1 = tmp0 + tmp3;
		z2 = tmp1 + tmp2;
		z3 = tmp0 + tmp2;
		z4 = tmp1 + tmp3;
		z5 = (z3 + z4) * FIX_1_175875602; 
		tmp0 = tmp0 * FIX_0_298631336; 
		tmp1 = tmp1 * FIX_2_053119869;
		tmp2 = tmp2 * FIX_3_072711026;
		tmp3 = tmp3 * FIX_1_501321110;
		z1 = z1 * (- FIX_0_899976223);
		z2 = z2 * (- FIX_2_562915447);
		z3 = z3 * (- FIX_1_961570560);
		z4 = z4 * (- FIX_0_390180644);
		z3 += z5;
		z4 += z5;
		tmp0 += z1 + z3;
		tmp1 += z2 + z4;
		tmp2 += z2 + z3;
		tmp3 += z1 + z4;
		wsptr[0] = (int) DESCALE(tmp10 + tmp3, CONST_BITS-PASS1_BITS);
		wsptr[56] = (int) DESCALE(tmp10 - tmp3, CONST_BITS-PASS1_BITS);
		wsptr[8] = (int) DESCALE(tmp11 + tmp2, CONST_BITS-PASS1_BITS);
		wsptr[48] = (int) DESCALE(tmp11 - tmp2, CONST_BITS-PASS1_BITS);
		wsptr[16] = (int) DESCALE(tmp12 + tmp1, CONST_BITS-PASS1_BITS);
		wsptr[40] = (int) DESCALE(tmp12 - tmp1, CONST_BITS-PASS1_BITS);
		wsptr[24] = (int) DESCALE(tmp13 + tmp0, CONST_BITS-PASS1_BITS);
		wsptr[32] = (int) DESCALE(tmp13 - tmp0, CONST_BITS-PASS1_BITS);
		inptr++;
		wsptr++;
		}
		wsptr = workspace;
		for (ctr = 0; ctr < 8; ctr++) {
			outptr = (char*)(output_buf + ctr*8);
	#ifndef NO_ZERO_ROW_TEST
			if ((wsptr[1] | wsptr[2] | wsptr[3] | wsptr[4] | wsptr[5] | wsptr[6] | wsptr[7]) == 0)
			{	char dcval;
			itp = (int) DESCALE((INT32) wsptr[0], PASS1_BITS+3)+128;
			if(itp > MAXJSAMPLE)
				itp = MAXJSAMPLE;
			else if(itp < 0)
				itp = 0;
			dcval = (char)itp;
			outptr[0] = dcval;
			outptr[1] = dcval;
			outptr[2] = dcval;
			outptr[3] = dcval;
			outptr[4] = dcval;
			outptr[5] = dcval;
			outptr[6] = dcval;
			outptr[7] = dcval;
			wsptr += 8;
			continue;
			}
	#endif
			z2 = (INT32) wsptr[2];
			z3 = (INT32) wsptr[6];
			z1 = (z2 + z3) * FIX_0_541196100;
			tmp2 = z1 + z3 * (- FIX_1_847759065);
			tmp3 = z1 + z2 * FIX_0_765366865;
			tmp0 = ((INT32) wsptr[0] + (INT32) wsptr[4]) << CONST_BITS;
			tmp1 = ((INT32) wsptr[0] - (INT32) wsptr[4]) << CONST_BITS;
			tmp10 = tmp0 + tmp3;
			tmp13 = tmp0 - tmp3;
			tmp11 = tmp1 + tmp2;
			tmp12 = tmp1 - tmp2;
			tmp0 = (INT32) wsptr[7];
			tmp1 = (INT32) wsptr[5];
			tmp2 = (INT32) wsptr[3];
			tmp3 = (INT32) wsptr[1];
			z1 = tmp0 + tmp3;
			z2 = tmp1 + tmp2;
			z3 = tmp0 + tmp2;
			z4 = tmp1 + tmp3;
			z5 = (z3 + z4) * FIX_1_175875602;
			tmp0 = tmp0 * FIX_0_298631336;
			tmp1 = tmp1 * FIX_2_053119869;
			tmp2 = tmp2 * FIX_3_072711026;
			tmp3 = tmp3 * FIX_1_501321110;
			z1 = z1 * (- FIX_0_899976223);
			z2 = z2 * (- FIX_2_562915447);
			z3 = z3 * (- FIX_1_961570560);
			z4 = z4 * (- FIX_0_390180644);
			z3 += z5;
			z4 += z5;
			tmp0 += z1 + z3;
			tmp1 += z2 + z4;
			tmp2 += z2 + z3;
			tmp3 += z1 + z4;
			itp = (int) DESCALE(tmp10 + tmp3,CONST_BITS+PASS1_BITS+3)+128;
			RANGE_LIMIT(itp);
			outptr[0] = (char)itp;
			itp = (int) DESCALE(tmp10 - tmp3,CONST_BITS+PASS1_BITS+3)+128;
			RANGE_LIMIT(itp);
			outptr[7] = (char)itp;
			itp = (int) DESCALE(tmp11 + tmp2,CONST_BITS+PASS1_BITS+3)+128;
			RANGE_LIMIT(itp);
			outptr[1] = (char)itp;
			itp = (int) DESCALE(tmp11 - tmp2,CONST_BITS+PASS1_BITS+3)+128;
			RANGE_LIMIT(itp);
			outptr[6] = (char)itp;
			itp = (int) DESCALE(tmp12 + tmp1,CONST_BITS+PASS1_BITS+3)+128;
			RANGE_LIMIT(itp);
			outptr[2] = (char)itp;
			itp = (int) DESCALE(tmp12 - tmp1,CONST_BITS+PASS1_BITS+3)+128;
			RANGE_LIMIT(itp);
			outptr[5] = (char)itp;
			itp = (int) DESCALE(tmp13 + tmp0,CONST_BITS+PASS1_BITS+3)+128;
			RANGE_LIMIT(itp);
			outptr[3] = (char)itp;
			itp = (int) DESCALE(tmp13 - tmp0,CONST_BITS+PASS1_BITS+3)+128;
			RANGE_LIMIT(itp);
			outptr[4] = (char)itp;
			wsptr += 8;
		}
	}

	void idct_float(short* coef_block,BYTE *output_buf)
	{	
		float tmp0, tmp1, tmp2, tmp3;
		float tmp10, tmp11, tmp12, tmp13;
		float z1, z2, z3, z4, z5;
		int itp;
		short *inptr;
		float * wsptr;
		char *outptr;
		int ctr;
		float workspace[64];
		INT32 shift_temp;

		inptr = coef_block;
		wsptr = workspace;
		for (ctr = 8; ctr > 0; ctr--)
		{	if ((inptr[8] | inptr[16] | inptr[24] | inptr[32] | inptr[40] | inptr[48] |
		inptr[56]) == 0) 
		{	float dcval = inptr[0];
		wsptr[0] = dcval;
		wsptr[8] = dcval;
		wsptr[16] = dcval;
		wsptr[24] = dcval;
		wsptr[32] = dcval;
		wsptr[40] = dcval;
		wsptr[48] = dcval;
		wsptr[56] = dcval;
		inptr++;
		wsptr++;
		continue;
		}
		z2 = inptr[16];
		z3 = inptr[48];
		z1 = (z2 + z3)*(float)0.541196100;
		tmp2 = z1 + z3*(float)(-1.847759065);
		tmp3 = z1 + z2*(float)0.765366865;
		z2 = inptr[0];
		z3 = inptr[32];
		tmp0 = (z2 + z3);
		tmp1 = (z2 - z3);
		tmp10 = tmp0 + tmp3;
		tmp13 = tmp0 - tmp3;
		tmp11 = tmp1 + tmp2;
		tmp12 = tmp1 - tmp2;
		tmp0 = inptr[56];
		tmp1 = inptr[40];
		tmp2 = inptr[24];
		tmp3 = inptr[8];
		z1 = tmp0 + tmp3;
		z2 = tmp1 + tmp2;
		z3 = tmp0 + tmp2;
		z4 = tmp1 + tmp3;
		z5 = (z3 + z4) * (float)1.175875602; 
		tmp0 = tmp0 * (float)0.298631336; 
		tmp1 = tmp1 * (float)2.053119869;
		tmp2 = tmp2 * (float)3.072711026;
		tmp3 = tmp3 * (float)1.501321110;
		z1 = z1 * (float)(-0.899976223);
		z2 = z2 * (float)(-2.562915447);
		z3 = z3 * (float)(-1.961570560);
		z4 = z4 * (float)(-0.390180644);
		z3 += z5;
		z4 += z5;
		tmp0 += z1 + z3;
		tmp1 += z2 + z4;
		tmp2 += z2 + z3;
		tmp3 += z1 + z4;
		wsptr[0] = tmp10 + tmp3;
		wsptr[56] = tmp10 - tmp3;
		wsptr[8] = tmp11 + tmp2;
		wsptr[48] = tmp11 - tmp2;
		wsptr[16] = tmp12 + tmp1;
		wsptr[40] = tmp12 - tmp1;
		wsptr[24] = tmp13 + tmp0;
		wsptr[32] = tmp13 - tmp0;
		inptr++;
		wsptr++;
		}
		wsptr = workspace;
		for (ctr = 0; ctr < 8; ctr++) {
			outptr = (char*)(output_buf + ctr*8);
			z2 = wsptr[2];
			z3 = wsptr[6];
			z1 = (z2 + z3) * (float)0.541196100;
			tmp2 = z1 + z3 * (float)(-1.847759065);
			tmp3 = z1 + z2 * (float)0.765366865;
			tmp0 = wsptr[0] + wsptr[4];
			tmp1 = wsptr[0] - wsptr[4];
			tmp10 = tmp0 + tmp3;
			tmp13 = tmp0 - tmp3;
			tmp11 = tmp1 + tmp2;
			tmp12 = tmp1 - tmp2;
			tmp0 = wsptr[7];
			tmp1 = wsptr[5];
			tmp2 = wsptr[3];
			tmp3 = wsptr[1];
			z1 = tmp0 + tmp3;
			z2 = tmp1 + tmp2;
			z3 = tmp0 + tmp2;
			z4 = tmp1 + tmp3;
			z5 = (z3 + z4) * (float)1.175875602;
			tmp0 = tmp0 * (float)0.298631336;
			tmp1 = tmp1 * (float)2.053119869;
			tmp2 = tmp2 * (float)3.072711026;
			tmp3 = tmp3 * (float)1.501321110;
			z1 = z1 * (float)(-0.899976223);
			z2 = z2 * (float)(-2.562915447);
			z3 = z3 * (float)(-1.961570560);
			z4 = z4 * (float)(-0.390180644);
			z3 += z5;
			z4 += z5;
			tmp0 += z1 + z3;
			tmp1 += z2 + z4;
			tmp2 += z2 + z3;
			tmp3 += z1 + z4;
			itp = (int) DESCALE((INT32)(tmp10 + tmp3),3)+128;
			RANGE_LIMIT(itp);
			outptr[0] = (char)itp;
			itp = (int) DESCALE((INT32)(tmp10 - tmp3),3)+128;
			RANGE_LIMIT(itp);
			outptr[7] = (char)itp;
			itp = (int) DESCALE((INT32)(tmp11 + tmp2),3)+128;
			RANGE_LIMIT(itp);
			outptr[1] = (char)itp;
			itp = (int) DESCALE((INT32)(tmp11 - tmp2),3)+128;
			RANGE_LIMIT(itp);
			outptr[6] = (char)itp;
			itp = (int) DESCALE((INT32)(tmp12 + tmp1),3)+128;
			RANGE_LIMIT(itp);
			outptr[2] = (char)itp;
			itp = (int) DESCALE((INT32)(tmp12 - tmp1),3)+128;
			RANGE_LIMIT(itp);
			outptr[5] = (char)itp;
			itp = (int) DESCALE((INT32)(tmp13 + tmp0),3)+128;
			RANGE_LIMIT(itp);
			outptr[3] = (char)itp;
			itp = (int) DESCALE((INT32)(tmp13 - tmp0),3)+128;
			RANGE_LIMIT(itp);
			outptr[4] = (char)itp;
			wsptr += 8;
		}
	}
};

inline
cls_image a_load_jpg(string str_filename)
{
	cls_format_jpg jpg(str_filename,IDCT_FLOAT);
	tag_jpg_info info;
	jpg.get_image_info(&info);

	cls_matrix<unsigned> mat_image(info.width,info.height);

	for(int i=0;i<info.width;i++)
	{
		for(int j=0;j<info.height;j++)
		{
			mat_image(i,j) = get_rgb(jpg.image[0][j*info.buf_width + i],jpg.image[1][j*info.buf_width + i],jpg.image[2][j*info.buf_width + i]);
		}
	}

	return cls_image(mat_image);
}

inline
cls_image& a_load_jpg(string str_filename,cls_image& img)
{
	cls_format_jpg jpg(str_filename,IDCT_FLOAT);
	tag_jpg_info info;
	jpg.get_image_info(&info);

	cls_matrix<unsigned> mat_image(info.width,info.height);

	for(int i=0;i<info.width;i++)
	{
		for(int j=0;j<info.height;j++)
		{
			mat_image(i,j) = get_rgb(jpg.image[0][j*info.buf_width + i],jpg.image[1][j*info.buf_width + i],jpg.image[2][j*info.buf_width + i]);
		}
	}

	img.assign_from_matrix(mat_image);

	return img;
}