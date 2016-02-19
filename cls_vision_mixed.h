#include "lib_import.h"

/*
	THIS CLASS IS AIMED AT THE MIXED OF COLOR;
*/

class cls_vision_mixed
	:public virtual cls_vision_basis
{
public:
	//͸���ȣ�mixed opacity;
	void mixed_opacity(const cls_image& that,double d_this,cls_image& des) const
	{
		check_identifical(*this,that);

		des.resize(get_width(),get_height());

		for(int i=0;i<get_width();i++)
		{
			for(int j=0;j<get_height();j++)
			{
				des.set_channel_value_0(i,j,get_channel_value_0(i,j)*d_this + (1-d_this)*that.get_channel_value_0(i,j));
				des.set_channel_value_1(i,j,get_channel_value_1(i,j)*d_this + (1-d_this)*that.get_channel_value_1(i,j));
				des.set_channel_value_2(i,j,get_channel_value_2(i,j)*d_this + (1-d_this)*that.get_channel_value_2(i,j));
				des.set_channel_value_3(i,j,get_channel_value_3(i,j)*d_this + (1-d_this)*that.get_channel_value_3(i,j));
			}
		}
	}

	//�䰵��daraken the image;
	void mixed_darken(const cls_image& that,cls_image& des) const
	{
		check_identifical(*this,that);

		des.resize(get_width(),get_height());

		for(int i=0;i<get_width();i++)
		{
			for(int j=0;j<get_height();j++)
			{
				des.set_channel_value_0(i,j,min(get_channel_value_0(i,j),that.get_channel_value_0(i,j)));
				des.set_channel_value_1(i,j,min(get_channel_value_1(i,j),that.get_channel_value_1(i,j)));
				des.set_channel_value_2(i,j,min(get_channel_value_2(i,j),that.get_channel_value_2(i,j)));
				des.set_channel_value_3(i,j,min(get_channel_value_3(i,j),that.get_channel_value_3(i,j)));
			}
		}
	}

	//������lighten the image;
	void mixed_lighten(const cls_image& that,cls_image& des) const
	{
		check_identifical(*this,that);

		des.resize(get_width(),get_height());

		for(int i=0;i<get_width();i++)
		{
			for(int j=0;j<get_height();j++)
			{
				des.set_channel_value_0(i,j,max(get_channel_value_0(i,j),that.get_channel_value_0(i,j)));
				des.set_channel_value_1(i,j,max(get_channel_value_1(i,j),that.get_channel_value_1(i,j)));
				des.set_channel_value_2(i,j,max(get_channel_value_2(i,j),that.get_channel_value_2(i,j)));
				des.set_channel_value_3(i,j,max(get_channel_value_3(i,j),that.get_channel_value_3(i,j)));
			}
		}
	}

	//�����Ƭ�������õ�Ƭ������һ��Ȼ���ӳ��͸�����Ҫ�ֱ�ͨ���������õ�Ƭ���Ӷ������������Ρ�
	void mixed_multiply(const cls_image& that,cls_image& des) const
	{
		check_identifical(*this,that);

		des.resize(get_width(),get_height());

		for(int i=0;i<get_width();i++)
		{
			for(int j=0;j<get_height();j++)
			{
				des.set_channel_value_0(i,j,(get_channel_value_0(i,j)*that.get_channel_value_0(i,j))/255.0 + 0.5);
				des.set_channel_value_1(i,j,(get_channel_value_1(i,j)*that.get_channel_value_1(i,j))/255.0 + 0.5);
				des.set_channel_value_2(i,j,(get_channel_value_2(i,j)*that.get_channel_value_2(i,j))/255.0 + 0.5);
				des.set_channel_value_3(i,j,(get_channel_value_3(i,j)*that.get_channel_value_3(i,j))/255.0 + 0.5);
			}
		}
	}

	//��ɫ���ͺ�����̨ͶӰ���ֱ������һ��ͼ�����ͶӰ��Ȼ��Ͷ�䵽ͬһ����Ļ��
	void mixed_screen(const cls_image& that,cls_image& des) const
	{
		check_identifical(*this,that);

		des.resize(get_width(),get_height());

		for(int i=0;i<get_width();i++)
		{
			for(int j=0;j<get_height();j++)
			{
				des.set_channel_value_0(i,j,get_suitable_byte(255.0 - (255.0 - get_channel_value_0(i,j))*( 255.0 - that.get_channel_value_0(i,j))/255.0 + 0.5));
				des.set_channel_value_1(i,j,get_suitable_byte(255.0 - (255.0 - get_channel_value_1(i,j))*( 255.0 - that.get_channel_value_1(i,j))/255.0 + 0.5));
				des.set_channel_value_2(i,j,get_suitable_byte(255.0 - (255.0 - get_channel_value_2(i,j))*( 255.0 - that.get_channel_value_2(i,j))/255.0 + 0.5));
				des.set_channel_value_3(i,j,get_suitable_byte(255.0 - (255.0 - get_channel_value_3(i,j))*( 255.0 - that.get_channel_value_3(i,j))/255.0 + 0.5));
			}
		}
	}

	//��ɫ��������ģʽ�£��ϲ�����Ⱦ������²�ı�¶�̶ȡ�����ϲ�Խ�����²��ȡ�Ĺ�Խ�࣬Ҳ����Խ����
	void mixed_color_dodge(const cls_image& that,cls_image& des) const
	{
		check_identifical(*this,that);

		des.resize(get_width(),get_height());

		for(int i=0;i<get_width();i++)
		{
			for(int j=0;j<get_height();j++)
			{
				des.set_channel_value_0(i,j,get_suitable_byte(get_channel_value_0(i,j)/(256.0 - that.get_channel_value_0(i,j))* 255.0 + 0.5));
				des.set_channel_value_1(i,j,get_suitable_byte(get_channel_value_1(i,j)/(256.0 - that.get_channel_value_1(i,j))* 255.0 + 0.5));
				des.set_channel_value_2(i,j,get_suitable_byte(get_channel_value_2(i,j)/(256.0 - that.get_channel_value_2(i,j))* 255.0 + 0.5));
				des.set_channel_value_3(i,j,get_suitable_byte(get_channel_value_3(i,j)/(256.0 - that.get_channel_value_3(i,j))* 255.0 + 0.5));
			}
		}
	}

	//��ɫ����:��ģʽ����һ��ģʽ�պ��෴������ϲ�Խ�������²��ȡ�Ĺ�Խ�٣�����ϲ�Ϊȫ��ɫ�����²�Խ�ڣ�����ϲ�Ϊȫ��ɫ�����������Ӱ���²㡣��������ĵط���������²������ֵ��
	void mixed_color_burn(const cls_image& that,cls_image& des) const
	{
		check_identifical(*this,that);

		des.resize(get_width(),get_height());

		for(int i=0;i<get_width();i++)
		{
			for(int j=0;j<get_height();j++)
			{
				des.set_channel_value_0(i,j,get_suitable_byte(255 - (255.0 -that.get_channel_value_0(i,j))/get_channel_value_0(i,j)*255.0 + 0.5));
				des.set_channel_value_1(i,j,get_suitable_byte(255 - (255.0 -that.get_channel_value_1(i,j))/get_channel_value_1(i,j)*255.0 + 0.5));
				des.set_channel_value_2(i,j,get_suitable_byte(255 - (255.0 -that.get_channel_value_2(i,j))/get_channel_value_2(i,j)*255.0 + 0.5));
				des.set_channel_value_3(i,j,get_suitable_byte(255 - (255.0 -that.get_channel_value_3(i,j))/get_channel_value_3(i,j)*255.0 + 0.5));
			}
		}
	}

	//���Լ����������²��ɫ��ֵ��ӡ������������
	void mixed_linear_dodge(const cls_image& that,cls_image& des) const
	{
		des = that + (*this);
	}

	//���μ���:������²������ֵ֮��С��255�������������Ǵ���ɫ��������ϲ㷴�࣬������Ǵ������ѧ����
	void mixed_linear_burn(const cls_image& that,cls_image& des) const
	{
		check_identifical(*this,that);

		des.resize(get_width(),get_height());

		for(int i=0;i<get_width();i++)
		{
			for(int j=0;j<get_height();j++)
			{
				des.set_channel_value_0(i,j,get_suitable_byte(get_channel_value_0(i,j) + that.get_channel_value_0(i,j) - 255));
				des.set_channel_value_1(i,j,get_suitable_byte(get_channel_value_1(i,j) + that.get_channel_value_1(i,j) - 255));
				des.set_channel_value_2(i,j,get_suitable_byte(get_channel_value_2(i,j) + that.get_channel_value_2(i,j) - 255));
				des.set_channel_value_3(i,j,get_suitable_byte(get_channel_value_3(i,j) + that.get_channel_value_3(i,j) - 255));
			}
		}
	} 

private:
	static byte overlay(byte a,byte b)
	{
		if (b < 127)
			return get_suitable_byte(2*a*b / 256.0 + 0.5);
		else
			return get_suitable_byte(255.0 - 2*(255-a)*(255-b)/255 + 0.5);
	}

	static byte hard_light(byte a,byte b)
	{
		if (a < 127)
			return 2*a*b / 256.0;
		else
			return get_suitable_byte(255.0 - 2*(255-a)*(255-b)/255 + 0.5);
	}

	static byte soft_light(byte a,byte b)
	{
		if (a < 127)
			return get_suitable_byte((2*a - 255.0)*(256.0*b - b*b) / 256.0 / 256.0 + b);
		else
			return get_suitable_byte((2*a - 255)*(sqrt(b/255.0)*255.0 - b)/255.0 + b);
	}

	static byte vivid_light(byte a,byte b)
	{
		if (a < 127)
			return get_suitable_byte(255.0 - (255.0 - b) / (2 * a) * 255.0);
		else
			return get_suitable_byte(b / (2*1.0*(256 - a)) * 255);
	}

	static byte linear_light(byte a,byte b)
	{
		return get_suitable_byte(b + 2 * a - 255);
	}

	static byte pin_light(byte a,byte b)
	{
		if (b < 2*a - 255)
			return get_suitable_byte(2*a - 255.0);
		else if (b > 2*a)
			return get_suitable_byte(2*a);
		else
			return b;
	}

	static byte hard_mix(byte a,byte b)
	{
		if (a < 255 - b)
			return 0;
		else
			return 255;
	}

	static byte difference(byte a,byte b)
	{
		return labs(a - b);
	}

	static byte exclusion(byte a,byte b)
	{
		return get_suitable_byte(a + b - 2 * a * b);
	}

public:

	//���ӣ������²�ɫ��ֵ�Ĳ�ͬ����ģʽ������Multiply��Ҳ������Screenģʽ��
	void mixed_overlay(const cls_image& that,cls_image& des) const
	{
		check_identifical(*this,that);

		des.resize(get_width(),get_height());

		for(int i=0;i<get_width();i++)
		{
			for(int j=0;j<get_height();j++)
			{
				des.set_channel_value_0(i,j,overlay(get_channel_value_0(i,j),that.get_channel_value_0(i,j) ));
				des.set_channel_value_1(i,j,overlay(get_channel_value_1(i,j),that.get_channel_value_1(i,j) ));
				des.set_channel_value_2(i,j,overlay(get_channel_value_2(i,j),that.get_channel_value_2(i,j) ));
				des.set_channel_value_3(i,j,overlay(get_channel_value_3(i,j),that.get_channel_value_3(i,j) ));
			}
		}
	} 

	//ǿ�⣺��ģʽ��ȫ���Ӧ��Overlayģʽ�£�����ͼ����д��򽻻������������ϲ����ɫ����50%�ң����²�Խ������֮Խ����
	void mixed_hard_light(const cls_image& that,cls_image& des) const
	{
		check_identifical(*this,that);

		des.resize(get_width(),get_height());

		for(int i=0;i<get_width();i++)
		{
			for(int j=0;j<get_height();j++)
			{
				des.set_channel_value_0(i,j,hard_light(get_channel_value_0(i,j),that.get_channel_value_0(i,j) ));
				des.set_channel_value_1(i,j,hard_light(get_channel_value_1(i,j),that.get_channel_value_1(i,j) ));
				des.set_channel_value_2(i,j,hard_light(get_channel_value_2(i,j),that.get_channel_value_2(i,j) ));
				des.set_channel_value_3(i,j,hard_light(get_channel_value_3(i,j),that.get_channel_value_3(i,j) ));
			}
		}
	} 

	//��⣺��ģʽ�����ϲ���Gammaֵ��ΧΪ2.0��0.5�ķ�ʽ�������²��ɫ��ֵ���������һ���ǳ���͵���ϡ�
	void mixed_soft_light(const cls_image& that,cls_image& des) const
	{
		check_identifical(*this,that);

		des.resize(get_width(),get_height());

		for(int i=0;i<get_width();i++)
		{
			for(int j=0;j<get_height();j++)
			{
				des.set_channel_value_0(i,j,soft_light(get_channel_value_0(i,j),that.get_channel_value_0(i,j) ));
				des.set_channel_value_1(i,j,soft_light(get_channel_value_1(i,j),that.get_channel_value_1(i,j) ));
				des.set_channel_value_2(i,j,soft_light(get_channel_value_2(i,j),that.get_channel_value_2(i,j) ));
				des.set_channel_value_3(i,j,soft_light(get_channel_value_3(i,j),that.get_channel_value_3(i,j) ));
			}
		}
	} 

	//���⣺��ģʽ�ǳ�ǿ�ҵ������˶Աȶȣ��ر����ڸ�������������������Ϊ��������Ӧ��Color Burn�͸�����Ӧ��Color Dodge��
	void mixed_vivid_light(const cls_image& that,cls_image& des) const
	{
		check_identifical(*this,that);

		des.resize(get_width(),get_height());

		for(int i=0;i<get_width();i++)
		{
			for(int j=0;j<get_height();j++)
			{
				des.set_channel_value_0(i,j,vivid_light(get_channel_value_0(i,j),that.get_channel_value_0(i,j) ));
				des.set_channel_value_1(i,j,vivid_light(get_channel_value_1(i,j),that.get_channel_value_1(i,j) ));
				des.set_channel_value_2(i,j,vivid_light(get_channel_value_2(i,j),that.get_channel_value_2(i,j) ));
				des.set_channel_value_3(i,j,vivid_light(get_channel_value_3(i,j),that.get_channel_value_3(i,j) ));
			}
		}
	} 

	//���Թ⣺�����ǰһ��ģʽ���ԣ���ģʽ���ӵĶԱȶ�Ҫ��Щ����������Linear Burn,ֻ�����Ǽ������ϲ��Ӱ������
	void mixed_linear_light(const cls_image& that,cls_image& des) const
	{
		check_identifical(*this,that);

		des.resize(get_width(),get_height());

		for(int i=0;i<get_width();i++)
		{
			for(int j=0;j<get_height();j++)
			{
				des.set_channel_value_0(i,j,linear_light(get_channel_value_0(i,j),that.get_channel_value_0(i,j) ));
				des.set_channel_value_1(i,j,linear_light(get_channel_value_1(i,j),that.get_channel_value_1(i,j) ));
				des.set_channel_value_2(i,j,linear_light(get_channel_value_2(i,j),that.get_channel_value_2(i,j) ));
				des.set_channel_value_3(i,j,linear_light(get_channel_value_3(i,j),that.get_channel_value_3(i,j) ));
			}
		}
	} 

	//���Դ����ģʽ������ǵ����м�������ǲ�����²㣬����������Darken��Lighte��ģʽ����ϡ�
	void mixed_pin_light(const cls_image& that,cls_image& des) const
	{
		check_identifical(*this,that);

		des.resize(get_width(),get_height());

		for(int i=0;i<get_width();i++)
		{
			for(int j=0;j<get_height();j++)
			{
				des.set_channel_value_0(i,j,pin_light(get_channel_value_0(i,j),that.get_channel_value_0(i,j) ));
				des.set_channel_value_1(i,j,pin_light(get_channel_value_1(i,j),that.get_channel_value_1(i,j) ));
				des.set_channel_value_2(i,j,pin_light(get_channel_value_2(i,j),that.get_channel_value_2(i,j) ));
				des.set_channel_value_3(i,j,pin_light(get_channel_value_3(i,j),that.get_channel_value_3(i,j) ));
			}
		}
	} 

	//ʵɫ��ϣ���ģʽ���������ս��������6�ֻ�����ɫ��ÿ��ͨ��Ҫô����0��Ҫô����255��
	void mixed_hard_mix(const cls_image& that,cls_image& des) const
	{
		check_identifical(*this,that);

		des.resize(get_width(),get_height());

		for(int i=0;i<get_width();i++)
		{
			for(int j=0;j<get_height();j++)
			{
				des.set_channel_value_0(i,j,hard_mix(get_channel_value_0(i,j),that.get_channel_value_0(i,j) ));
				des.set_channel_value_1(i,j,hard_mix(get_channel_value_1(i,j),that.get_channel_value_1(i,j) ));
				des.set_channel_value_2(i,j,hard_mix(get_channel_value_2(i,j),that.get_channel_value_2(i,j) ));
				des.set_channel_value_3(i,j,hard_mix(get_channel_value_3(i,j),that.get_channel_value_3(i,j) ));
			}
		}
	}

	//��ֵ�����²�ɫ���ľ���ֵ����ģʽ��Ҫ���ڱȽ�������ͬ�汾��ͼƬ�����������ȫһ��������Ϊȫ�ڡ�
	void mixed_difference(const cls_image& that,cls_image& des) const
	{
		check_identifical(*this,that);

		des.resize(get_width(),get_height());

		for(int i=0;i<get_width();i++)
		{
			for(int j=0;j<get_height();j++)
			{
				des.set_channel_value_0(i,j,difference(get_channel_value_0(i,j),that.get_channel_value_0(i,j) ));
				des.set_channel_value_1(i,j,difference(get_channel_value_1(i,j),that.get_channel_value_1(i,j) ));
				des.set_channel_value_2(i,j,difference(get_channel_value_2(i,j),that.get_channel_value_2(i,j) ));
				des.set_channel_value_3(i,j,difference(get_channel_value_3(i,j),that.get_channel_value_3(i,j) ));
			}
		}
	}

	//�ų�������ͼƬ���򽫵�����һ��ķ��࣬�ܰ��������򽫵�����һ����ȫû�иı䡣
	void mixed_exclusion(const cls_image& that,cls_image& des) const
	{
		check_identifical(*this,that);

		des.resize(get_width(),get_height());

		for(int i=0;i<get_width();i++)
		{
			for(int j=0;j<get_height();j++)
			{
				des.set_channel_value_0(i,j,exclusion(get_channel_value_0(i,j),that.get_channel_value_0(i,j) ));
				des.set_channel_value_1(i,j,exclusion(get_channel_value_1(i,j),that.get_channel_value_1(i,j) ));
				des.set_channel_value_2(i,j,exclusion(get_channel_value_2(i,j),that.get_channel_value_2(i,j) ));
				des.set_channel_value_3(i,j,exclusion(get_channel_value_3(i,j),that.get_channel_value_3(i,j) ));
			}
		}
	}
};