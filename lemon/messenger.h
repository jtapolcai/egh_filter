///==============================================================
/// \file  messenger.h
/// \brief Implemenation of streams for warnings, errors and logs
///
///--------------------------------------------------------------
/// begin                July 15, 2005
/// last update          18 08 2005
/// @author              Gabor Nemeth and Janos Tapolcai
/// email                ng201@mail2.selye.sk, tapolcai@tmit.bme.hu
/////////////////////////////////////////////////////////////////

#ifndef _MESSENGER_H_
#define _MESSENGER_H_

#include <cstdio>
#include <iostream>

extern unsigned logging_priority_level;
extern unsigned number_of_errors;
extern const bool LOG_MSG;

#undef HTML_LOG

/// for historical reasons - the previous compiler give the entire path for __FILE__, 
/// instead displaying the filename only
inline const char* _extractFilename(const char *path){
	const char *last=path;
    while(*path++) if(*path=='/' || *path=='\\') last=path+1;
	return last;
}

#define XML(tag,value) "<"<<tag<<">"<<value<<"</"<<tag<<">\n"

#define LOGLEV(i) logging_priority_level+=i;


#ifdef HTML_LOG

#define FIGDIR "./fig/"
/*#define log_msg_start "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0 Transitional//EN\">\n<HTML>\n"<<\
	"<head><script>\nfunction goline(log,line) {\n"<<\
	"if (document.body.createTextRange) {\n"<<\
	"var range = frames[log].document.body.createTextRange();\n"<<\
	"if (range.findText(line)) { range.select(); }\n"<<\
	"} else {  frames[log].find(line); }\n}\n</script></head>\n<BODY>\n"*/

//#define log_msg_start "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0 Transitional//EN\">\n<HTML>\n"<<\
//	"<head><script>\nfunction goline(log,line) {\n"<<\
//	"myWindow=window.open (log); myWindow.window.scrollByLines(line);\n}\n</script></head>\n<BODY>\n"

#define log_msg_start 
//cout<<"<A name=\"phpl0_0\"></A>"<<"<A name=\"phpl1_0\"></A>"<<"<A name=\"phpl2_0\"></A>"\n;
//cout<<"<CODE>\n"

//"<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0 Transitional//EN\">\n<HTML>\n"<<\
//	"<head></head>\n<BODY><CODE>\n"


//scrollTo (0, line)

#define log_msg_finish 
//cout<<"</CODE>"
//cout<<"</CODE></BODY>\n</HTML><\n"

/// Error function/macro
#define error_msg number_of_errors++;\
       std::cout<<"\n<br/><a href=\""<<__FILE__ << "#" << __LINE__ <<"\"><h3>Error</h3></a> " 

//extern int php_level[6];

#include <sstream>
/*std::string make_string(int i){
	std::ostringstream streamOut;
	streamOut << i; 
	return streamOut.str();
}*/

#define OutFileOpened(os,name) if (!os) { error_msg<<"Error opening "<<name; exit (-1);\
                            } else { std::cout<<"\n<br/><a href=\""<<name<<"\">File opend "<<name<<"</a>";}


#define anchor_html(level) ""
//"<a href=\"#phpl"<<level<<"_"<<(php_level[level]-1)<<"\">&uArr;</a>"

//  <<"<a href=\"#phpl"<<level<<"_"<<(php_level[level]+1)<<"\">&dArr;</a>"

/// Debug printing function/macro
#define log_msg(priority) if(LOG_MSG && priority<=logging_priority_level)\
       std::cout<<"\n<br/><a href=\""<<__FILE__ << "#" << __LINE__ <<"\">&copy;</a>"<<anchor_html(1)\
      <<((priority==2)?"&nbsp;&nbsp;&nbsp;&nbsp;":(priority==3)?"&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;":(priority>=4)?"&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;":"&nbsp;&nbsp;") 

//       <<anchor_html(1)<<"<A name=\"phpl"<<priority<<"_"<<(php_level[priority]++)<<"\"></A>"\

//     ((priority==2)?"&nbsp;&nbsp;":(priority==3)?"&nbsp;&nbsp;&nbsp;":(priority>=4)?"&nbsp;&nbsp;&nbsp;&nbsp;":"&nbsp;") \
//       std::cout<<"\n<br/><button onclick=\"goline(\'"<<__FILE__<<"\',"<<__LINE__<<")\">code</button> "\

#define bookmark ""
//<A name=\"phpl1_"<<(php_level[1]++)<<"\"></A>" 

#else 
#define FIGDIR "./fig/"
#define log_msg_start
#define log_msg_finish "program exited normaly"

/// Error function/macro
#define error_msg number_of_errors++;\
       std::cout<<"\nError at [" <<_extractFilename(__FILE__) << ":" << __LINE__ << "] " 

#define OutFileOpened(os,name) if (!os) {error_msg<<"Error opening "<<name; exit (-1);};

#ifdef SHORT_LOG
#define log_msg(priority) if(LOG_MSG && priority<=logging_priority_level)\
       std::cout<<"\n"<<((priority==2)?"  ":(priority==3)?"    ":(priority>=4)?"      ":" ") 

#else
/// Debug printing function/macro
#define log_msg(priority) if(LOG_MSG && priority<=logging_priority_level)\
       std::cout<<"\n"<<((priority==2)?"  ":(priority==3)?"    ":(priority>=4)?"      ":" ") <<"[" <<_extractFilename(__FILE__) << ":" << __LINE__ << "] " 
#endif

#endif

#endif
