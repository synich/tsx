//ע���ߣ�xuegang (qq:308821698 blog: http://www.cppblog.com/flysnowxg)
//ԭʼ���룺http://tinyscheme.sourceforge.net/home.html
#pragma once
#include <stdio.h>
#pragma warning(disable:4996)

struct cell_t;
struct scheme;

struct num_t {
	bool is_fix;//�Ƿ�������
	union {
		long i;//����
		double f;//������
	};
} ;

struct string_t{
	char *_str;
	int _len;
};

typedef cell_t* (*foreign_func_t)(scheme *, cell_t*); //�ⲿ����չ����ԭ��

struct pait_t{
	struct cell_t *_car;
	struct cell_t *_cdr;
};

enum e_port_subclass {
	port_free=0,
	port_file=1,
	port_string=2,
	port_can_realloc=4,
	port_input=16,
	port_output=32,
	port_eof=64
};

struct port_t {
	unsigned char kind;
	union {
		struct {
			FILE *file;
			char *filename;
			int curr_line;
			int closeit;
		} f;
		struct {
			char *start;
			char *end;//λ�����һ���ַ�֮��
			char *curr;
		}s;
	};
};

struct cell_t {
	unsigned int _flag;
	union {
		num_t _num;
		string_t _string;
		foreign_func_t _fun;
		pait_t _pair;
		port_t *_port;
	};
};

#define CELL_SEGSIZE 5000		/* # of cells in one segment */
#define CELL_NSEGMENT 100    /* # of segments for cells */
#define MAX_FILES 256
#define STR_BUFF_SIZE 1024

typedef void * (*func_alloc)(size_t);
typedef void (*func_dealloc)(void *);

extern cell_t g_nil;
extern cell_t g_true;
extern cell_t g_false;
extern cell_t g_eof;

struct scheme {
	//�ڴ�������
	func_alloc malloc;
	func_dealloc free;
	cell_t* cell_seg[CELL_NSEGMENT];
	int last_cell_seg;
	cell_t* free_cell;				/* cell* to top of free cells */
	long free_cell_count;		/* # of free cells */

	//�˿ڹ���
	cell_t* outport;
	cell_t* inport;
	cell_t* save_inport;
	cell_t* loadport;
	port_t load_stack[MAX_FILES];		//���ǿ��ܻ�ݹ�ļ����ļ����ַ������� (load "xx.ss") ,���ﱣ���˵ݹ���ص��ļ�ʱ�γɵ�ջ
	int nesting_stack[MAX_FILES];		//�����˷���ÿ���ļ�ʱ���ʽ�ĵݹ���
	int top_file_index;

	/* We use 4 registers. */
	int op;							//��ǰ������
	cell_t* args;					/* register for arguments of function */
	cell_t* envir;					/* stack register for current environment */ 
	cell_t* code;					/* register for current code */
	cell_t* call_stack;			/* stack register for next evaluation */
	cell_t* ret_value;
	int nesting;

	cell_t* oblist;				/* cell* to symbol table */		//�������еķ��ţ�ȷ����������ͬ���ֵķ�����ͬһ��
	cell_t* global_env;		/* cell* to global environment */

	int tok;						//���ִʷ�������ȡ�ĵ���
	char strbuff[STR_BUFF_SIZE];
	int eval_result;			//����������������״̬ ���������˳��󣬿���ͨ�����ֵ֪���������˳���ԭ��
	int print_flag;			//����atom2str�����д�ӡ����ĸ�ʽ
	char gc_verbose;      /* if gc_verbose is not zero, print gc status */

	/* global cell*s to special symbols */
	cell_t* sym_lambda;			/* cell* to syntax lambda */
	cell_t* sym_quote;				/* cell* to syntax quote */						//����    '
	cell_t* sym_qquote;			/* cell* to symbol quasiquote */			//׼���� `
	cell_t* sym_unquote;			/* cell* to symbol unquote */				//������ ,
	cell_t* sym_unquote_sp;	/* cell* to symbol unquote-splicing */	//������ ,@
	cell_t* sym_feed_to;			/* => */												// cond�����õ�
	cell_t* sym_colon_hook;		/* *colon-hook* */
	cell_t* sym_error_hook;		/* *error-hook* */
	cell_t* sym_sharp_hook;		/* *sharp-hook* */
	cell_t* sym_compile_hook;	/* *compile-hook* */
};

/* operator code */
enum opcode {
#define _OP_DEF(A,B,C,D,E,OP) OP,
#include "opdefines.h"
	OP_MAXDEFINED
};

typedef cell_t* (*dispatch_func_t)(scheme *, opcode);
typedef struct {
	dispatch_func_t func;	//��������
	char *name;					//������
	int min_arity;					//���ٲ�������
	int max_arity;				//����������
	char *args_type;
} op_code_info;
