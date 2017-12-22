#include<bits/stdc++.h>
using namespace std;

map<string,string> SYMTAB;
map<string,string> OPTAB;
string address, finishAddress, programName, len;
int start, finish;

#define N 112345

string memory[N];
int memorymap[N];

#define op   0
#define ad1  1
#define ad2  2
#define resb 3
#define resw 4
#define word 5
#define byte 6
#define S second

bool comp(string a,string b)
{
	if(a.size() < b.size())
        return true;
	else if(a.size() > b.size())
        return false;
	else
	{
		for(int i = 0; i < a.size(); ++i)
		{
			if(b[i] > a[i])return true;
			else if(b[i] < a[i])return false;
		}
		return true;
	}
}
char intvalue(int a)
{
	if(a < 10)
		return char(a + '0');
	else
		return char((a - 10) + 'a');
}
int charvalue(char a)
{
	if(a >= 'a' && a <= 'f')
		return int(a - 'a' + 10);
	else if(a >= 'A' && a <= 'F')
		return int(a - 'A' + 10);
	else
		return int(a - '0');
}
int X = 0;
string decToHex(int n)
{
	string str;
	while(n > 0)
	{
		str += intvalue(n & 15);
		n >>= 4;
	}
	string ans;
	for(int i = str.size() - 1; i >= 0; --i)
	{
		ans += str[i];
	}
	return ans;
}
int hexToDec(string n)
{
	int a = 0;
	for(int i = 0; i < n.size(); ++i)
	{
		a = (a<<4) + charvalue(n[i]);
	}
	return a;
}
void updateaddress(int v)
{
	int a = hexToDec(address);
	a += v;
	address = decToHex(a);
}
void printaddress()
{
	int j = 6 - address.size();
	for(int i = 0; i < j; ++i)
		printf("0");
	for(int i=0; i < address.size(); ++i)
	{
		printf("%c",address[i]);
	}
	printf("\t\t");
}
bool feasible(string s)
{
	int z = hexToDec(s);
	if(z >= (1<<15))
		z = X + z - (1<<15) ;
	if(z < finish - 2 && z >= start && memorymap[z] == -1 && memorymap[z+1] == -1 && memorymap[z+2] == -1)return true;
	else if(z < finish - 2 && z >= start && memorymap[z] == word)return true;
	else if(z < finish - 2 && z >= start && memorymap[z] == resw)return true;
	else return false;
}
bool feasibleChar(string s)
{
	int z = hexToDec(s);
	if(z >= (1<<15)) z = X + z - (1<<15) ;
	if(z < finish && z >= start && memorymap[z] == -1)return true;
	else if(z < finish && z >= start && memorymap[z] == byte)return true;
	else if(z < finish && z >= start && memorymap[z] == resb)return true;
	else return false;
}
bool operationAt(string s)
{
	int z = hexToDec(s);
	if(z >= (1<<15)) z = X + z - (1<<15);
	if(z < finish - 2 && z >= start && memorymap[z] == op)return true;
	else if(z < finish - 2 && z >= start && memorymap[z] == -1)
	{
		auto p = OPTAB.find(memory[z]);
		if(p != OPTAB.end() && memorymap[z+1] == -1 && memorymap[z+2] == -1 && feasible(memory[z+1] + memory[z+2]))
		{
			memorymap[z] = op;
			memorymap[z+1] = ad1;
			memorymap[z+2] = ad2;
			return true;
		}
		else return false;
	}
	else return false;
}
string cur = "000";
string newLabel()
{
	int c = hexToDec(cur);
	c += 13;
	cur = decToHex(c);
	string ans = "LBL";
	ans += cur;
	return ans;
}
string current = "000";
string newIOLabel()
{
	int c = hexToDec(current);
	c += 11;
	current = decToHex(c);
	string ans = "IO_";
	ans += current;
	return ans;
}
char line[100];
int main()
{

	OPTAB["00"] = "LDA";
	OPTAB["18"] = "ADD";
	OPTAB["40"] = "AND";
	OPTAB["28"] = "COMP";
	OPTAB["24"] = "DIV";
	OPTAB["3C"] = "J";
	OPTAB["30"] = "JEQ";
	OPTAB["38"] = "JLT";
	OPTAB["34"] = "JGT";
	OPTAB["48"] = "JSUB";
	OPTAB["50"] = "LDCH";
	OPTAB["08"] = "LDL";
	OPTAB["04"] = "LDX";
	OPTAB["20"] = "MUL";
	OPTAB["44"] = "OR";
	OPTAB["D8"] = "RD";
	OPTAB["4C"] = "RSUB";		//don't print m
	OPTAB["0C"] = "STA";
	OPTAB["54"] = "STCH";
	OPTAB["14"] = "STL";
	OPTAB["10"] = "STX";
	OPTAB["1C"] = "SUB";
	OPTAB["E0"] = "TD";
	OPTAB["2C"] = "TIX";
	OPTAB["DC"] = "WD";

	for(int i = 0; i < N; ++i)
	{
		memorymap[i] = -1;
		memory[i] = "00";
	}

	while(1)
	{
		scanf("%s", line);
		if(line[0] == 'H')
		{
			for(int i = 7; i < 13; ++i)
			{
				address += line[i];
			}
			for(int i = 1; i < 7; ++i)
			{
				programName += line[i];
			}
			for(int i = 13; i < 19; ++i)
			{
				len += line[i];
			}
			finishAddress = decToHex(hexToDec(address) + hexToDec(len));

		}
		else if(line[0] == 'E')
		{
			break;
		}
		else if(line[0] == 'T')
		{
			string add;
			for(int i = 1; i < 7; ++i)
			{
				add += line[i];
			}
			int beg = hexToDec(add);
			string length;
			length += line[7];
			length += line[8];
			int l = hexToDec(length);
			for(int i = 0, j = beg; i < l; ++i, ++j)
			{
				memory[j].clear();
				memory[j] += line[i * 2 + 9];
				memory[j] += line[i * 2 + 10];
			}
		}
		else
		{
			printf("Object File has some problems.\n");
			return 0;
		}
	}

	//algorithm
	start = hexToDec(address);
	finish = hexToDec(finishAddress);

	//separate operations dealing with a single byte		STCH LDCH
	for(int i=start;i<finish-2;++i)
	{
		if(memorymap[i] >= 0)
			continue;
		string a;
		a += memory[i+1];
		a += memory[i+2];
		if((memory[i] == "50" || memory[i] == "54") && feasibleChar(a))
		{
			memorymap[i] = op;
			memorymap[i+1] = ad1;
			memorymap[i+2] = ad2;
			SYMTAB[a] = newLabel();
			if(memorymap[hexToDec(a)]<0)
			{
				if(memory[i] == "50")memorymap[hexToDec(a)] = byte;
				else memorymap[hexToDec(a)] = resb;
			}
		}
	}

	for(int i = start; i < finish; ++i)
	{
		if(memorymap[i] >= 0)continue;
		auto p = OPTAB.find(memory[i]);
		string a;
		a += memory[i+1];
		a += memory[i+2];
		if(p!=OPTAB.end())
		{
			string str = p->S;
			if(str=="TD" || str=="WD" || str == "RD")
			{
				if(feasibleChar(a))
				{
					memorymap[i] = op;
					memorymap[i+1] = ad1;
					memorymap[i+2] = ad2;
					SYMTAB[a] = newIOLabel();

					int x = hexToDec(a);
					if(memorymap[x]<0)
					{
						memorymap[x] = byte;
					}
				}
			}
			else if(str[0]!='J' && memorymap[i+1] == -1 && memorymap[i+2] == -1 && feasible(a))
			{
				memorymap[i] = op;
				memorymap[i+1] = ad1;
				memorymap[i+2] = ad2;
				SYMTAB[a] = newLabel();
				int x = hexToDec(a);
				if(memorymap[x]<0)
				{
					memorymap[x] = word;
					memorymap[x+1] = word;
					memorymap[x+2] = word;
				}
			if(memory[x] == "00 "&& memory[x+1] == "00" && memory[x+2] == "00" && (p->S == "WD" |p->S=="STL"||p->S=="STX"||p->S=="STA"))
				{
					memorymap[x] = resw;
					memorymap[x+1] = resw;
					memorymap[x+2] = resw;
				}

				//update X
				if(str=="LDX")
				{
					X = hexToDec(memory[hexToDec(a)]);
				}
			}
			else if(str[0] == 'J' && operationAt(a))		//J type instructions
			{
				SYMTAB[a] = newLabel();
				memorymap[i] = op;
				memorymap[i+1] = ad1;
				memorymap[i+2] = ad2;
			}
			else if(str == "RSUB")
			{
				memorymap[i] = op;
				memorymap[i+1] = ad1;
				memorymap[i+2] = ad2;
				SYMTAB[a]= "  ";
			}
		}
		else
		{

		}
	}

	//printing
	printf("\t\t");
	cout<<programName<<"\t\t";
	cout<<"START\t\t";
	cout<<address<<"\n";
	SYMTAB[address] = "FIRST";
	for(int i=start;i<finish;++i)
	{
		printaddress();
		auto p = SYMTAB.find(address);
		if(p != SYMTAB.end())cout<<SYMTAB[address];

		if(memorymap[i] == op)
		{
			cout<<"\t\t";
			cout<<OPTAB[memory[i]]<<"\t\t";
			cout<<SYMTAB[memory[i+1] + memory[i+2]];
			if(hexToDec(memory[i+1] + memory[i+2]) >= (1<<15))cout<<",X";
			cout<<"\n";
			i += 2;
			updateaddress(3);
		}
		else if(memorymap[i]==word)
		{
			if(p==SYMTAB.end())
			{
				SYMTAB[ decToHex(i) ] = newLabel();
				cout<<SYMTAB[decToHex(i)];
			}
			cout<<"\t\tWORD\t\t";
			cout<<hexToDec(memory[i]+memory[i+1]+memory[i+2])<<"\n";
			i += 2;
			updateaddress(3);
		}
		else if(memorymap[i]==byte)
		{
			if(p==SYMTAB.end())
			{
				SYMTAB[ decToHex(i) ] = newLabel();
				cout<<SYMTAB[decToHex(i)];
			}
			cout<<"\t\tBYTE\t\t"<<hexToDec(memory[i]);
			if(hexToDec(memory[i])>='A'&&hexToDec(memory[i])<='Z')cout<<" = "<<char(hexToDec(memory[i]));
			cout<<"\n";
			updateaddress(1);
		}
		else if(memorymap[i]==resw)
		{
			cout<<"\t\tRESW\t\t";
			int ct = 0;
			while(memorymap[i]==resw)
			{
				i += 3;
				++ct;
			}
			cout<<ct<<"\n";
			--i;
			updateaddress(ct*3);
		}
		else if(memorymap[i]==resb)
		{
			cout<<"\t\tRESB\t\t";
			int ct = 0;
			while(i<finish&&memorymap[i]==resb)
			{
				++ct;
				++i;
			}
			cout<<ct<<"\n";
			--i;
			updateaddress(ct);
		}
		else if(memorymap[i]==-1)
		{
			if(memory[i]=="00")
			{
				int ct = 0;
				while((i<finish)&&(memorymap[i]==resb||memorymap[i]==-1))
				{
					++ct;
					++i;
				}
				cout<<newLabel()<<"\t\tRESB\t\t"<<ct<<"\n";
				--i;
				updateaddress(ct);
			}

			else
			{
				cout<<newLabel()<<"\t\tBYTE\t\t"<<hexToDec(memory[i]);
				if(hexToDec(memory[i])>='A'&&hexToDec(memory[i])<='Z')cout<<" = "<<char(hexToDec(memory[i]));
				cout<<"\n";
				updateaddress(1);
			}
		}
		else
		{
			cerr<<memorymap[i]<<endl;
		}
	}
	printf("\t\t\t\tEND\t\tFIRST\n");
	return 0;
}
