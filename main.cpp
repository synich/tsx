#include "scheme.cpp"

int main(int argc, char **argv) {
	scheme* sc = scheme_new(malloc,free);
	if(NULL == sc) {
		fprintf(stderr,"Could not initialize!\n");
		return -1;
	}
	FILE* fin=fopen("init.scm","r");
	if(fin!=0) scheme_load_file(sc,fin,"init.scm");

	if((argc>1) && strcmp(argv[1],"-?")==0) {
		printf("TinyScheme 1.41\n");
		printf("帮助 :	tinyscheme -?\n");
		printf("启动交互式控制台：tinyscheme\n");
		printf("执行一个文件：tinyscheme  <file> [<arg1> <arg2> ...]\n");
		printf("执行一个字符串：tinyscheme  -s command\n");
		return 1;
	}
	else if((argc>1)&&(strcmp(argv[1],"-s")==0)){
		scheme_load_string(sc,argv[2]);
	}
	else if(argc>1){
		char* file_name=argv[1];
		cell_t* args=&g_nil;
		argv += 2;
		for(; *argv; argv++) {
			cell_t* p=mk_string(sc,*argv);
			args=cons(sc,p,args);
		}
		args=reverse(sc,args);
		scheme_define(sc,sc->global_env,mk_symbol(sc,"*args*"),args);
		FILE* fin=fopen(file_name,"r");
		if(fin==0) fprintf(stderr,"Could not open file %s\n",file_name);
		else scheme_load_file(sc,fin,file_name);
	}
	else{
		printf("TinyScheme 1.41\n");
		scheme_load_file(sc,stdin,0);
	}
	scheme_destroy(sc);
	return sc->eval_result;
}
