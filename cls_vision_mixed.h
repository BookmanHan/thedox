#include "lib_import.h"

/*
	THIS CLASS IS AIMED AT THE MIXED OF COLOR;
*/

class cls_vision_mixed
	:public virtual cls_vision_basis
{
public:
	//透明度：mixed opacity;
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

	//变暗：daraken the image;
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

	//变亮：lighten the image;
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

	//正像叠片：两个幻灯片叠加在一起然后放映，透射光需要分别通过这两个幻灯片，从而被削弱了两次。
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

	//滤色：就好像两台投影机分别对其中一个图层进行投影后，然后投射到同一个屏幕上
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

	//颜色减淡：该模式下，上层的亮度决定了下层的暴露程度。如果上层越亮，下层获取的光越多，也就是越亮。
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

	//颜色加深:该模式和上一个模式刚好相反。如果上层越暗，则下层获取的光越少，如果上层为全黑色，则下层越黑，如果上层为全白色，则根本不会影响下层。结果最亮的地方不会高于下层的像素值。
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

	//线性减淡：将上下层的色彩值相加。结果将更亮。
	void mixed_linear_dodge(const cls_image& that,cls_image& des) const
	{
		des = that + (*this);
	}

	//线形加深:如果上下层的像素值之和小于255，输出结果将会是纯黑色。如果将上层反相，结果将是纯粹的数学减。
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

	//叠加：依据下层色彩值的不同，该模式可能是Multiply，也可能是Screen模式。
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

	//强光：该模式完全相对应于Overlay模式下，两个图层进行次序交换的情况。如过上层的颜色高于50%灰，则下层越亮，反之越暗。
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

	//柔光：该模式类似上层以Gamma值范围为2.0到0.5的方式来调制下层的色彩值。结果将是一个非常柔和的组合。
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

	//亮光：该模式非常强烈的增加了对比度，特别是在高亮和阴暗处。可以认为是阴暗处应用Color Burn和高亮处应用Color Dodge。
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

	//线性光：相对于前一种模式而言，该模式增加的对比度要弱些。其类似于Linear Burn,只不过是加深了上层的影响力。
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

	//电光源：该模式结果就是导致中间调几乎是不变的下层，但是两边是Darken和Lighte年模式的组合。
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

	//实色混合：该模式导致了最终结果仅包含6种基本颜色，每个通道要么就是0，要么就是255。
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

	//差值：上下层色调的绝对值。该模式主要用于比较两个不同版本的图片。如果两者完全一样，则结果为全黑。
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

	//排除：亮的图片区域将导致另一层的反相，很暗的区域则将导致另一层完全没有改变。
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