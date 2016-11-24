#include <bits/stdc++.h>
using namespace std;

#define dbg(x) cout<<#x<<"="<<x<<endl

typedef vector< vector<string> > table;
typedef  vector<string>   tup;
#define iter ::iterator 


int attrNo=5;


struct dtree
{
	string attr;
	map<string,dtree*>child;
};

typedef dtree* dptr;

pair<tup,table> get_input()
{
	tup tupl,attrList;
	table tupleList;
	ifstream file;
	string s="";
	string n="";
	file.open("inputf.in");
	for(int i=0;i<attrNo;i++)
	{
		file>>n;
		attrList.push_back(n);

	}
	while(!file.eof())
	{
		for(int i=0;i<attrNo;i++)
		{
			file>>n;
			tupl.push_back(n);
			//cout<<n<<" ";

		}
		if(file.eof())
		{
			break;
		}
		 

		tupleList.push_back(tupl);
		tupl.clear();

	}
	return make_pair(attrList,tupleList);
}

bool checkSameClass(table &tempTable)
{
	int last=tempTable[0].size()-1;
	for(int i=0;i<tempTable.size();i++)
	{
		if(tempTable[i][last]!=tempTable[0][last])
			return 0;
	}
	return 1;

}

string get_Major_Class(table &tempTable)
{
	int last=tempTable[0].size()-1;
	map<string,int>mp;
	int maxNo=-1;
	string maxClass="";
	for(int i=0;i<tempTable.size();i++)
	{
		mp[tempTable[i][last]]++;
		if(mp[tempTable[i][last]]>maxNo)
		{
			maxNo=mp[tempTable[i][last]];
			maxClass=tempTable[i][last];
		}
	}
	return maxClass ;
}

double Info(table &tempTable)
{
	map<string,int>mp;
	int totalsize=tempTable.size();
	int last=tempTable[0].size()-1;
	double ans=0;

	for(int i=0;i<totalsize;i++)
	{
		 
		mp[tempTable[i][last]]++;
	}
	for(map<string ,int>iter it=mp.begin();it!=mp.end();it++)
	{

		double prob= (double(it->second))/totalsize;
		 
		ans-=(prob)*log2(prob);
		  

	}
	 
	return ans;
}


map<string,table> getPartition(table &tempTable,int index,string attr)
{

	map<string,table>mp;
	for(int i=0;i<tempTable.size();i++)
	{

		mp[tempTable[i][index]].push_back(tempTable[i]);
	}
	return mp;
}

pair<string,int> Attrbute_Selection(table &tempTable,tup &attrList,map<string,bool>&Hash)
{
	double curinfo=Info(tempTable);
	 
	double maxgain=-2,curgain=-2;
	double partinfo=0;
	string maxAttr="";
	int maxInd=-1;
	for(int i=0;i<attrList.size()-1;i++)
	{
		if(Hash[attrList[i]]==false)
		{
			 
			 partinfo=0;
			 map<string,table>mp=getPartition(tempTable,i,attrList[i]);
			 
			for(map<string,table>iter it=mp.begin();it!=mp.end();it++)
			{
				int sz=(it->second).size();
				double prob=double(sz)/tempTable.size();
				 
				partinfo+=prob*Info(it->second);
			}
			
			curgain=curinfo-partinfo;
		  
			if(curgain>maxgain)
			{
				maxgain=curgain;
				maxAttr=attrList[i];
				maxInd=i;
			}

		}
	}

	 
	return make_pair(maxAttr,maxInd);

}

int level=0;
dptr create_Tree(table &tempTable,tup &attrList,map<string,bool>&Hash)
{
	level++;
	 
	if(tempTable.size()==0)
	{
		level--;
		 
		return NULL;
	}
	if(checkSameClass(tempTable))
	{
		 
		level--;
		dptr curNode=new dtree();
		curNode->attr=tempTable[0][tempTable[0].size()-1]; //cur clas
		 
		return curNode;
	}
 
	int flag=0;
	for(int i=0;i<attrList.size();i++)
	{
		if(Hash[attrList[i]]==false)
			flag=1;
	}
	 
	if(flag==0)
	{
		 
		level--;
		dptr curNode=new dtree();
		string majorClass=get_Major_Class(tempTable);  //max class
		curNode->attr=majorClass;

		 
		return curNode;
	}
	//attribute Selection
	dptr curNode=new dtree();
	pair<string,int> attrpair=Attrbute_Selection(tempTable,attrList,Hash);
	string attr=attrpair.first;
	Hash[attr]=true;
	curNode->attr=attr;
	 
	cout<<attr<<"{";

	map<string,table>part=getPartition(tempTable,attrpair.second,attr);
	for(map<string,table>iter it=part.begin();it!=part.end();it++)
	{
		     
			cout<<it->second[0][attrpair.second]<<",->\n";
		      
			dptr childNode=create_Tree(it->second,attrList,Hash);
			if(childNode==NULL)
			{
				string majorClass=get_Major_Class(tempTable); 
				childNode=new dtree();
				childNode->attr=majorClass;
			}
			curNode->child[it->first]=childNode;
	}
	cout<<"}";
	Hash.erase(attr);
	level--;
	 return curNode;

}

void TravelTree(dptr head)
{
	queue<dptr>qu;
	qu.push(head);
	cout<<head->attr<<"\n ";
	while(!qu.empty())
	{
		dptr cur=qu.front();qu.pop();

		for(map<string,dptr>iter it=cur->child.begin();it!=cur->child.end();it++)
		{
				dptr chil=it->second;
				cout<<it->first<<" ";
				qu.push(chil);
		}


	}
}






int main()
{
	pair<tup,table>input=get_input();
	map<string,bool>Hash;
	tup attrList=input.first;
	table tupleList=input.second;
	 
	dptr head=create_Tree(tupleList,attrList,Hash);
	 
}