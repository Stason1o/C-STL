#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <utility>
#include <algorithm>
using namespace std::rel_ops;
typedef std::string						string_t;
typedef std::vector<string_t>			string_vector;
typedef std::vector<size_t>				size_vector;
typedef std::ifstream					ifstream_t;

const string_t PATH = "D:\lab1.txt";

struct Record {
	size_t id;
	string_t type;			// Тип публикации
	string_t name;			// Имя публикации
	string_t date;			// Дата публикации
	string_t place;			// Место публикации
	string_vector authors;	// Авторы публикации

	Record() : id(0), type(""), name(""), date(""), place("") {
		authors.clear();
	}

	Record(size_t _id, string_t _type, string_t _name, string_t _date, string_t _place, string_vector _authors) {//констпуктор копирования
		init(_id, _type, _name, _date, _place, _authors);
	}

	Record(const Record& r) {
		init(r.id, r.type, r.name, r.date, r.place, r.authors);
	}

	void init(size_t _id, string_t _type, string_t _name, string_t _date, string_t _place, string_vector _authors) {
		id = _id;
		type = _type;
		name = _name;
		date = _date;
		place = _place;
		authors = _authors;
	}

	void readLine(std::istream& fin) {//считывание строки
		string_t line;
		std::getline(fin, line);

		std::stringstream lineStream(line);
		string_t item;

		std::getline(lineStream, item, ',');	id = std::stoi(item);
		std::getline(lineStream, item, ',');	type = item;
		std::getline(lineStream, item, ',');	name = item;
		std::getline(lineStream, item, ',');	date = item;
		std::getline(lineStream, item, ',');	place = item;

		authors.clear();
		while (std::getline(lineStream, item, ','))
			authors.push_back(item);
	}

	bool operator==(const Record& r) const { return id == r.id; }
	bool operator <(const Record& r) const { return id < r.id; }
	~Record() {};
};

std::ostream& operator<<(std::ostream& os, const Record& r) {
	os << r.id << "  " + r.type + "\t" +
		r.name + "  " + r.date + "\t" +
		r.place;
	for (auto it = r.authors.begin(); it != r.authors.end(); ++it)
		os << "  " << *it;
	os << std::endl;
	return os;
}

struct Data {
	ifstream_t file;

	Data() {
		file.open("lab1.txt", std::ios::in);
		if (!file.is_open())
			std::cout << "File not found" << std::endl;
	}

	Data(const string_t& filename) {
		file.open(filename, std::ios::in);
		if (!file.is_open())
			std::cout << "File " + filename + " not found" << std::endl;
	}

	~Data() {
		if (file.is_open())
			file.close();
		file.clear();
	}
};

struct Table : public Data {
	size_t n;	// Количество записей
	std::vector<Record> records;

	Table() : Data(), n(0) {}

	Table(const string_t& filename) : Data(filename) {
		records.clear();
		n = 0;
		Record tmp;
		while (!file.eof()) {
			tmp.readLine(file);
			records.push_back(tmp);
			++n;
		}
	}

	void add() {
		Record tmp;
		size_t nauth;
		string_t buf;

		tmp.id = ++n;
		std::cout << "Enter type: ";			std::cin >> tmp.type;
		std::cout << "Enter name: ";			std::cin >> tmp.name;
		std::cout << "Enter date(MM/DD/YYY): ";	std::cin >> tmp.date;
		std::cout << "Enter place: ";			std::cin >> tmp.place;
		std::cout << "How many authors?: ";		std::cin >> nauth;
		for (size_t i = 0; i < nauth; ++i) {
			std::cout << "Enter author N" << i + 1 << ": ";
			std::cin >> buf;
			tmp.authors.push_back(buf);
		}
		records.push_back(tmp);
	} //добавление записи

	void change(size_t _id) {//изменение записи
		auto it = records.begin();
		for (it; it != records.end(); ++it)
			if (it->id == _id) break;		// Нашли необходимую запись

		int n = 1;
		while (n) {
			size_t			num_tmp;
			string_t		str_tmp;
			string_vector	vec_tmp;

			std::cout << "1 - Change id" << std::endl;
			std::cout << "2 - Change type" << std::endl;
			std::cout << "3 - Change name" << std::endl;
			std::cout << "4 - Change date" << std::endl;
			std::cout << "5 - Change place" << std::endl;
			std::cout << "6 - Change authors" << std::endl;

			size_t choise = 0;
			std::cout << ">";	std::cin >> choise;
			switch (choise) {
			case 1:
				std::cout << "Enter the ID to change : ";
				std::cin >> num_tmp;
				it->id = num_tmp;
				break;

			case 2:
				std::cout << "Enter the type to change: ";
				std::cin >> str_tmp;
				it->type = str_tmp;
				break;

			case 3:
				std::cout << "Enter the name to change: ";
				std::cin >> str_tmp;
				it->name = str_tmp;
				break;

			case 4:
				std::cout << "Enter the date to change(MM/DD/YYYY): ";
				std::cin >> str_tmp;
				it->date = str_tmp;
				break;

			case 5:
				std::cout << "Enter the place to change: ";
				std::cin >> str_tmp;
				it->place = str_tmp;
				break;

			case 6:
				std::cout << "How many authors: ";
				std::cin >> num_tmp;
				it->authors.clear();
				for (size_t i = 0; i < num_tmp; ++i){
					std::cin >> str_tmp;
					it->authors.push_back(str_tmp);
				}

			default:
				std::cout << "Enter the number between 1-6" << std::endl;
				break;
			}

			std::cout << "Continue? (1 - yes / 0 - no)" << std::endl;
			std::cin >> n;
		}
	}

	void del(string_t _name) { //удаление записи
		auto it = records.begin();
		for (it; it != records.end(); ++it)
			if (it->name == _name) {
				records.erase(it);
				break;
			}
		if (it == records.end()) {
			std::cout << "No such record" << std::endl;
			return;
		}
	}

	void searchByName(string_t _name){//поиск по названию публикации
		for (auto it = records.begin(); it != records.end(); ++it)
			if (it->name == _name){
				std::cout << *it;
				break;
			}
	}

	void searchByAuthor(string_t _author){//поиск по автору
		for (auto it = records.begin(); it != records.end(); ++it)
			for (auto jt = it->authors.begin(); jt != it->authors.end(); ++jt)
				if (*jt == _author)
					std::cout << *it;
	}

	void commit(const string_t& filename) { //запись в файл
		std::ofstream fout(filename);

		for (auto it = records.begin(); it != records.end(); ++it) {
			fout << it->id << "," << it->type << "," << it->name << ","
				<< it->date << "," << it->place;
			for (auto jt = it->authors.begin(); jt != it->authors.end(); ++jt)
				fout << "," << *jt;
			it != records.end() - 1 ? fout << "\n" : fout << "";
		}
	}

	~Table() {}
};

std::ostream& operator<<(std::ostream& os, const Table& t) {
	for (auto it = t.records.begin(); it != t.records.end(); ++it)
		os << *it;
	return os;
}

void menu(string_t filename) {
	Table t(filename);
	int n;
	int m = 0;
	while (m != 6){
		std::cout << "What do you want to do?:" << std::endl;
		std::cout << "1 - Add record;" << std::endl;
		std::cout << "2 - Edit record;" << std::endl;
		std::cout << "3 - Delete record;" << std::endl;
		std::cout << "4 - Search by name of record;" << std::endl;
		std::cout << "5 - Search by name of Author;" << std::endl;
		std::cout << "6 - Exit" << std::endl;
		std::cin >> n;
		string_t NAME;
		switch (n){
		case 1:
			t.add();
			t.commit(filename);
			break;
		case 2:
			size_t ID;
			std::cout << "Enter the ID of record, you wish to edit: "; std::cin >> ID;
			t.change(ID);
			break;
		case 3:
			std::cout << "Enter the NAME that contains record to delete: ";
			std::cin >> NAME;
			t.del(NAME);
			t.commit(filename);
			break;
		case 4:
			std::cout << "Enter the NAME of record you wish to find: ";
			std::cin >> NAME;
			t.searchByName(NAME);
			break;
		case 5:
			std::cout << "Enter the Name of Author you wish to find: ";
			std::cin >> NAME;
			t.searchByAuthor(NAME);
			break;
		case 6:
			m = 6;
		}
	}
}

const char* SQL = "CREATE TABLE IF NOT EXISTS foo(a,b,c); INSERT INTO FOO VALUES(1,2,3); INSERT INTO FOO SELECT * FROM FOO;";
const char* sql = "CREATE TABLE IF NOT EXISTS myDataBase( id integer,type varchar(20),name varchar(40),pubDate varchar(11),authName1 varchar(20),authName2 varchar(20),authName3 varchar(20)  )";
					 
const char* insert = "";
					 // "INSERT into myDataBase(id,type,name,pubDate,authName1) values (1, 'Detection', 'lunch at the cannibal', '2012-20-6','Daria Dontsova'); ";
					 //"INSERT into myDataBase(id,type,name,pubDate,authName1) values (2, 'Roman', 'War and Piece', '2010-3-5','Tolstoi L.N.');";
					 //"INSERT into myDataBase(id,type,name,pubDate,authName1) values (3, 'Satira', 'Dead Spirits', '1995-7-4','Gogoli N.V.');";
					 //"INSERT into myDataBase(id,type,name,pubDate,authName1) values (4, 'Fantastics', 'Master and Margarita', '2004-26-11','Bulgacov');";
					 //"INSERT into myDataBase(id,type,name,pubDate,authName1) values (5, 'Story', 'Heart of a Dog', '2006-7-3','Bulgacov');";
					 //"INSERT into myDataBase(id,type,name,pubDate,authName1) values (6, 'Satira', 'Revizor', '2011-12-11','Gogoli N.V.');";
					 //"INSERT into myDataBase(id,type,name,pubDate,authName1) values (7, 'Roman', 'Anna Karenina', '2003-1-1','Tolstoi L.N.');";
					 //"INSERT into myDataBase(id,type,name,pubDate,authName1) values (8, 'Story', 'Childhood', '1995-7-4','Gogoli N.V.');";
const char* findByName = "SELECT id,type,name,pubDate,authName1 FROM myDataBase where authName1 = ?";
const char* del = "drop table myDataBase";
const char* select = "SELECT * from myDataBase";


const char* query = "SELECT * FROM example";
const char* insert_query = "INSERT INTO example VALUES (?, ?, ?)";

int main() {
	menu(PATH);
	sqlite3* db;
	sqlite3_stmt* stmt;
	int result = sqlite3_open("my_database.dblite", &db);
	char* authName ;
	std::cin >> authName;

	/*sqlite3_prepare_v2(db, findByName , 255, &stmt, NULL);*/
	/*sqlite3_bind_int(stmt, 1, 3);
	sqlite3_bind_text(stmt, 2, "income", -1, NULL);
	sqlite3_bind_text(stmt, 3, "1488", -1, NULL);*/
	//result = sqlite3_step(stmt);
	//if (result != SQLITE_DONE){
	//	std::cout << "sqlite insertion error: " << sqlite3_errmsg(db) << std::endl;
	//}
	//sqlite3_finalize(stmt);

	sqlite3_prepare_v2(db, findByName, 255, &stmt, NULL);
	sqlite3_bind_text(stmt, 1 ,authName, -1, NULL);
	//sqlite3_bind_int(stmt, 1, 2);

	while ((result = sqlite3_step(stmt)) == SQLITE_ROW){
		std::cout << sqlite3_column_int(stmt, 0) << " "
			<< sqlite3_column_text(stmt, 1) << " "
			<< sqlite3_column_text(stmt, 2) << " "
			<< sqlite3_column_text(stmt, 3) << " "
			<< sqlite3_column_text(stmt, 4)	<< std::endl;
	}
	
	sqlite3_finalize(stmt);
	sqlite3_close(db);
	system("PAUSE");
	return 0;
	system("pause");
	return 0;
}