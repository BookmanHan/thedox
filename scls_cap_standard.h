#include "lib_import.h"

/*
	the cap standard,include:
	1.cap command
	2.cap variable type
*/
namespace cap_standard
{
	/*
		the different segment's meaning;
	*/

	//the wrong segment;
	const static unsigned short cap_segment_null = 0;
	//the const's segment;
	const static unsigned short cap_segment_const = 1;
	//the static's segment;
	const static unsigned short cap_segment_static = 2;
	//the stack's segment;
	const static unsigned short cap_segment_stack = 3;
	//the memory's segment;
	const static unsigned short cap_segment_memory = 4;
	//the codes' segment;
	const static unsigned short cap_segment_code = 5;
	//the things relative addr with "this";
	const static unsigned short cap_segment_relative = 6;
	/*
		the command does nothing;
	*/
	const static unsigned int cap_command_nop = 0;

	/*
		the basic pointer and assignment commands;
	*/
	//set a tag at here;
	const static unsigned int cap_command_tag = 1;
	//set operand_0 to result;
	const static unsigned int cap_command_set = 2;
	//get the operand_0's content to result;
	const static unsigned int cap_command_get = 3;
	//set the operand_0 to result's content;
	const static unsigned int cap_command_pointer_set = 4;
	//get the pointer_0's content to result's content; result.legnth => lenght of copying.
	const static unsigned int cap_command_pointer_get = 5;
	//set the operand_0's address to result;
	const static unsigned int cap_command_set_address = 53;

	/*
		the computing commands;
	*/
	const static unsigned int cap_command_add = 6;
	const static unsigned int cap_command_minus = 7;
	const static unsigned int cap_command_mutiply = 55;
	const static unsigned int cap_command_divide = 54;
	const static unsigned int cap_command_unsigned_mutiply = 8;
	const static unsigned int cap_command_unsigned_divide = 9;
	const static unsigned int cap_command_mod = 10;
	const static unsigned int cap_command_or = 11;
	const static unsigned int cap_command_and = 12;
	const static unsigned int cap_command_xor = 13;
	const static unsigned int cap_command_inc = 14;
	const static unsigned int cap_command_dec = 15;
	const static unsigned int cap_command_neg = 16;
	//the second offset is a real value of offset.
	const static unsigned int cap_command_pointer_real_add = 49;
	
	/*
		the logic commands;
	*/
	const static unsigned int cap_command_left = 17;
	const static unsigned int cap_command_right =18 ;
	const static unsigned int cap_command_roleft = 19;
	const static unsigned int cap_command_roright = 20;
	const static unsigned int cap_command_larger = 21;
	const static unsigned int cap_command_larger_eql = 22;
	const static unsigned int cap_command_smaller = 23;
	const static unsigned int cap_command_smaller_eql = 24;
	const static unsigned int cap_command_equ = 25;
	const static unsigned int cap_command_non_equ = 26;
	//nonsense
	const static unsigned int cap_command_assign_set = 27;
	const static unsigned int cap_command_not = 28;

	/*
		the control commands;
	*/
	const static unsigned int cap_command_jump = 29;
	const static unsigned int cap_command_abs_jump = 30;
	const static unsigned int cap_command_push = 31;
	const static unsigned int cap_command_pop = 32;
	//op  resulr.addr=call_address;
	const static unsigned int cap_command_call = 33;
	const static unsigned int cap_command_pre_call = 34;
	//op_0=return value
	const static unsigned int cap_command_post_call = 35;
	const static unsigned int cap_command_return = 36;
	const static unsigned int cap_command_get_retrun = 37;
	
	/*
		the loop commands for optimizing.
	*/
	const static unsigned int cap_command_loopend =38;
	const static unsigned int cap_command_for = 39;
	const static unsigned int cap_command_while = 40;
	const static unsigned int cap_command_do_while = 41;
	const static unsigned int cap_command_break = 42;
	const static unsigned int cap_command_continue = 43;

	/*
		the function commands;
	*/
	//op op_0.legnth = new_length; if (op_0.length == 0) op_1 == new_length;resulr.addr  =addresss;
	const static unsigned int cap_command_new = 44;
	const static unsigned int cap_command_delete = 45;

	//special use;
	const static unsigned int cap_command_memory_add_ref = 46;
	const static unsigned int cap_command_memory_delete_ref = 47;

	/*
		the loading command;
	*/
	const static unsigned int cap_command_load_dynamic = 48;

	/*
		the exception handle;
	*/
	const static unsigned int cap_command_throw = 50;
	const static unsigned int cap_command_aserror = 51;

	/*
		System running;
	*/
	const static unsigned int cap_command_syscall = 52;
	/*
		User definations;
	*/
	const static unsigned int cap_command_users = 1000;
};
