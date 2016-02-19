/*
	the different segment's meaning;
*/
//the wrong segment;
const static unsigned short cdox_segment_null = 0;
//the const's segment;
const static unsigned short cdox_segment_const = 1;
//the static's segment;
const static unsigned short cdox_segment_static = 2;
//the stack's segment;
const static unsigned short cdox_segment_stack = 3;
//the memory's segment;
const static unsigned short cdox_segment_memory = 4;
//the codes' segment;
const static unsigned short cdox_segment_code = 5;
//the things relative addr with "this";
const static unsigned short cdox_segment_relative = 6;
//the things in the base of stack;
const static unsigned short cdox_segment_base_stack = 6;

//the limit of segment of stack;
const static unsigned int limit_segment_stack = 1024 * 1024;
/*
	the different command's meaning;
*/

//the command will do nothing;
const static unsigned int cdox_command_nop = 0;

/*
	the basic pointer and assignment commands;
*/
//set a tag at here;
const static unsigned int cdox_command_tag = 1;
//set operand_0 to result;
const static unsigned int cdox_command_set = 2;
//get the operand_0's content to result;
const static unsigned int cdox_command_get = 3;
//set the operand_0 to result's content;
const static unsigned int cdox_command_pointer_set = 4;
//get the pointer_0's content to result's content; result.legnth => lenght of copying.
const static unsigned int cdox_command_pointer_get = 5;
//set the operand_0's address to result;
const static unsigned int cdox_command_set_address = 6;

/*
	the computing commands as their meanings.
*/
const static unsigned int cdox_command_add = 7;
const static unsigned int cdox_command_minus = 8;
const static unsigned int cdox_command_mutiply = 9;
const static unsigned int cdox_command_divide = 10;
const static unsigned int cdox_command_unsigned_mutiply = 11;
const static unsigned int cdox_command_unsigned_divide = 12;
const static unsigned int cdox_command_mod = 13;
const static unsigned int cdox_command_or = 14;
const static unsigned int cdox_command_and = 15;
const static unsigned int cdox_command_xor = 16;
const static unsigned int cdox_command_inc = 17;
const static unsigned int cdox_command_dec = 18;
const static unsigned int cdox_command_neg = 19;

/*
	the logic commands;
*/
const static unsigned int cdox_command_left = 20;
const static unsigned int cdox_command_right =21 ;
const static unsigned int cdox_command_roleft = 22;
const static unsigned int cdox_command_roright = 23;
const static unsigned int cdox_command_larger = 24;
const static unsigned int cdox_command_larger_eql = 25;
const static unsigned int cdox_command_smaller = 26;
const static unsigned int cdox_command_smaller_eql = 27;
const static unsigned int cdox_command_equ = 28;
const static unsigned int cdox_command_non_equ = 29;
const static unsigned int cdox_command_not = 30;

/*
	the control commands;
*/
const static unsigned int cdox_command_jump = 31;
const static unsigned int cdox_command_abs_jump = 32;
const static unsigned int cdox_command_push = 33;
const static unsigned int cdox_command_pop = 34;
//op  resulr.addr=call_address;
const static unsigned int cdox_command_call = 35;
const static unsigned int cdox_command_pre_call = 36;
//op_0=return value
const static unsigned int cdox_command_post_call = 37;
const static unsigned int cdox_command_return = 37;
const static unsigned int cdox_command_get_retrun = 38;

/*
	the loop commands for optimizing.
*/
const static unsigned int cdox_command_loopend =39;
const static unsigned int cdox_command_for = 40;
const static unsigned int cdox_command_while = 41;
const static unsigned int cdox_command_do_while = 42;
const static unsigned int cdox_command_break = 43;
const static unsigned int cdox_command_continue = 44;

/*
	the function commands;
*/
//op op_0.legnth = new_length; if (op_0.length == 0) op_1 == new_length;resulr.addr  =addresss;
const static unsigned int cdox_command_new = 45;
const static unsigned int cdox_command_delete = 46;

//special use;
const static unsigned int cdox_command_memory_add_ref = 47;
const static unsigned int cdox_command_memory_delete_ref = 48;

/*
	the loading command;
*/
const static unsigned int cdox_command_load_dynamic = 49;

/*
	the exception handle;
*/
const static unsigned int cdox_command_throw = 50;
const static unsigned int cdox_command_aserror = 51;

/*
	System running;
*/
const static unsigned int cdox_command_syscall = 52;

/*
	float point's computing;
*/
const static unsigned int cdox_command_float_add = 53;
const static unsigned int cdox_command_float_minus = 54;
const static unsigned int cdox_command_float_mutiply = 55;
const static unsigned int cdox_command_float_divide = 56;

/*
	User definations;
*/
const static unsigned int cdox_command_users_begin = 100;
const static unsigned int cdox_command_users_end = 500;
