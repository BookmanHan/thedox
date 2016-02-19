function cls_io()
{
	function cls_console()
	{
		naive "gets" as "console.gets";
		naive "puts" as "console.puts";
		naive "endline" as "console.endline";
		naive "get_num" as "console.get_num";
		naive "put_num" as "console.put_num";
	}

	function cls_file_text()
	{
		naive "open" as "file.open_text";
		naive "close" as "file.close_text";
		naive "write" as "file.write_text";
		naive "read" as "file.read_text";
 	}
	
	var file_text = new cls_file_text();
	var console = new cls_console();
}

var stdio = new cls_io();