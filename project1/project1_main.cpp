#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <map>
using namespace std;

string trim(
	string str,
	string whitespace = " \t\n\r"
){
	//Function used to strip extranious space from strings
    auto strBegin = str.find_first_not_of(whitespace);
    if (strBegin == std::string::npos)
        return ""; 

    auto strEnd = str.find_last_not_of(whitespace);
    auto strRange = strEnd - strBegin + 1;

    return str.substr(strBegin, strRange);
}

string extract_query_name(string input){
//Extract properly formated search name from input
        string temp,temp2, build;
        int pos = 0;
        int maxlength = 19;
        stringstream ss;
		// Adding a EOF terminator for simplicity
        ss << input << ' ' << "0";
        while(temp != "0"){
                ss >> temp;
                if (temp.find('-') != std::string::npos) {
                        pos=temp.find('-');
                        temp2 = temp.substr(0,pos+1);
                }
                if(build.length() + temp.length()+1 <= 19 && temp != "0"){
                        build += ' '+temp;
                }else if(build.length()+temp2.length()+1 && temp !="0" ){
						build +=' '+temp2;
						//Return trimmed
						return trim(build);
                }else 	return trim(build);
        }
}

multimap <string, double>extract_classes(string professor_querry){
	string course_number, student_count, professor_name;
	double d_student_count;
	ifstream myfile;
	//Map Class Number/Name to student count
	multimap <string, double> classes;
	myfile.open("class_sched_list.csv");
	if (myfile.is_open()){
		while ( myfile.good() ){
			//File deliminated by csv
			getline(myfile,course_number,',');
			getline(myfile,student_count,',');
			getline(myfile,professor_name,',');
			course_number = trim(course_number);
			d_student_count = atof(student_count.c_str());
			//Only classes for requested professor are stored
			if(professor_name == professor_querry){
				classes.insert( make_pair( course_number, d_student_count));
			}
		}
		myfile.close();
		return classes;
	} else cout << "Unable to open file"; 
}

multimap <string, double>extract_fees(){
	string course_number, s_fee;
	double fee = 0;
	ifstream myfile;
	multimap <string, double> fees;
	myfile.open("course_fees.csv");
	if (myfile.is_open()){
		while ( myfile.good() ){
			getline(myfile,course_number,',');
			getline(myfile,s_fee,',');
			fee = atof(s_fee.substr(1,string::npos).c_str());
			fees.insert( make_pair( trim(course_number),fee ) );
		}
		myfile.close();
		return fees;
	} else cout << "Unable to open file";
}

double calculate_income(
	multimap< string, double > classes,
	multimap< string, double > fees,
	string requested_professor
){
	double total_money = 0;
	double total_students = 0;
	//For each class in the classes map, students * fee (Fees map)
	for (
		map<string, double>::iterator its = classes.begin();
		its != classes.end();
		++its
	){		
		total_students += its->second;
		total_money += its->second * fees.find(its->first)->second;
	}	
	cout 		<< "----------------------------------------------" << endl
				<< "Professor: " << requested_professor 			<< endl
				<< "Current Total Students: " << total_students 	<< endl
				<< "Total Income: " << total_money 					<< endl
				<< "----------------------------------------------" << endl;
	return total_money;
}

int main(){
    string professor_querry,requested_professor;
	//User input (Scary Stuff)
	cout << "Professor Name? (Case & Punctuation Sensitive):\n";
    getline(cin,requested_professor);
	//Parse their input..
    professor_querry = trim(extract_query_name(requested_professor));
	//Build fee and classes maps
	multimap< string, double > classes = extract_classes(professor_querry);
	multimap< string, double > fees = extract_fees();
	//Calculate the professors income to the school
	calculate_income(classes, fees, requested_professor);
	//Exit successfull
	return 0;
}

