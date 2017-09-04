/* file name : ParameterReader.h
 * Author : 
 * Mail : 
 * ps : reading the parameter on program
 */
# pragma once
//C++
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
using namespace std;


//struct frame
struct FRAME
{
	string rgb,depth;
};

//load the parameter class
class ParameterReader
{
public:
		map<string, string> data;
		ParameterReader( string filename="/home/lhmstu/Image_publisher/src/image_publisher/parameter.txt")
		{
			ifstream fin( filename.c_str() );
			if(!fin)
			{
				cerr<<"file does not exist"<<endl;
				return ;
			}
			while(!fin.eof())
			{
				string str;
				getline( fin, str);
				if( str[0] == '#')
				{
					//# is p.s.
					continue;
				}
				
				int pos = str.find("=");
				if( pos == -1)
					continue;
				string key = str.substr( 0, pos );
				string value = str.substr( pos+1, str.length() );
				data[key] = value;
			
				if(!fin.good())
					break;
			}
		}
		string getData( string key )
		{
			map<string, string>::iterator iter = data.find(key);
			if( iter == data.end())
			{
				cout<<"Parameter name < "<< key <<" > not found!" <<endl;
				return string("NOT FOUND");
			}
			else
			{
				return iter->second;
			}
		}
};