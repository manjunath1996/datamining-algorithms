
#include <bits/stdc++.h>
#include <fstream>

#define dbg(x) cout<<#x<<" "<<x<<endl
#define N 100
using namespace std;


vector<int>v[N];
int coun[N];
map<int,int>mp;
int NoofItems,Support,NoofTransactions;
int mx=-1,no_t,no_i;
map<set<int>,int>GlobalSet;
map<set<int>,int>::iterator it;
set<int>::iterator jt;
 
#define ii pair<int,int> 

struct Node
{
	int item,freq,curc;
	bool mark;
	Node *cptr[N],*next,*parent;
	Node(int item_)
	{
		for(int i=0;i<N;i++)
			cptr[i]=NULL;
		freq=0;
		curc=0;
		mark=0;
		item=item_;
		next=NULL;
	}
};



bool comp1(ii a,ii b)
{
	if(a.second==b.second)
	{
		return a.first<b.first;
	}
	return a.second>b.second;
}

bool comp2(int a,int b)
{
 
	return (coun[a]>coun[b]);
}




int conv(string k)
{
	int j=0;
	for(int i=0;i<k.length();i++)
	{
		j=j*10+k[i]-'0';
	}
	return j;
	
}

void IncreaseFreq(Node *R)
{
	R->freq++;
}



void Attach(int Item,Node * R,Node** ItemPointers)
{
	 if(ItemPointers[Item]==NULL)
		ItemPointers[Item]=R;
	else
	{

		Node *K=ItemPointers[Item];
		while(K->next!=NULL)
			K=K->next;
		K->next=R;

	}

}

void createTree(Node *Root,vector<int>v,Node **ItemPointers,int count[])
{
	Node *R=Root;
	int idx=0;
	while(idx<v.size())
	{
		int Item=v[idx];
		if(R->cptr[Item]==NULL)
		{
			R->cptr[Item]=new Node(Item);
			R->cptr[Item]->parent=R;
			R=R->cptr[Item];

			Attach(Item,R,ItemPointers);
		}
		else
		{
			R=R->cptr[Item];
		}
		 
		IncreaseFreq(R);
		count[Item]++;
		idx++;

		
	}
}

void createReducedTree(Node *Root,vector<pair<int,int>>v,Node ** ItemPointers,int count[])
{
	Node *R=Root;
	int idx=0;
	while(idx<v.size())
	{
		int Item=v[idx].first;
		 
		if(R->cptr[Item]==NULL)
		{
			R->cptr[Item]=new Node(Item);
			R->cptr[Item]->parent=R;
			R=R->cptr[Item];
			count[Item]+=v[idx].second;
			R->freq+=v[idx].second;

			Attach(Item,R,ItemPointers);
		}
		else
		{
			R=R->cptr[Item];
			count[Item]+=v[idx].second;
			R->freq+=v[idx].second;
		}
		 
		
		
		idx++;

		
	}
}

void TraverseTree(Node *R)
{
	cout<<"Item "<<R->item<<"\n";
	cout<<"Childs: ";
	for(int i=0;i<N;i++)
	{
		if(R->cptr[i]!=NULL)
			cout<<i<<" ";
	}
	cout<<endl;
	for(int i=0;i<N;i++)
		if(R->cptr[i]!=NULL)
		TraverseTree(R->cptr[i]);

}

void PruneTree(Node *&Root,int Hash[])
{
	if(Root==NULL)
		return;
	else if(Hash[Root->item]<Support)              //changed 23/11/2016
		{
			Root=NULL;
			return;
		}
	else
		for(int i=0;i<N;i++)
			if(Root->cptr[i]!=NULL)
				PruneTree(Root->cptr[i],Hash);

}

void GenerateSets(vector<pair<int,int>>v,vector<int>PathSet,int count)
{
	int siz=v.size();
	set<int>subset;
	for(int i=0;i<PathSet.size();i++)
		subset.insert(PathSet[i]);

	for(int i=0;i<pow(2,siz);i++)
	{
		int number=i;
		int cur=0;
		int cnt=count;
		while(number)
		{
			if(number%2==1)
			{
				subset.insert(v[cur].first);
				cnt=min(cnt,v[cur].second);
			}
			number/=2;
			cur++;
		}
		if(subset.size()>0 && cnt>=Support)
		{
			GlobalSet.insert({subset,cnt});
		}
		number=i;
		cur=0;
		while(number)
		{
			if(number%2==1)
			{
				subset.erase(v[cur].first);
				 
			}
			number/=2;
			cur++;
		}




	}
}

 

void MineTree(Node *R,int Item,vector<int>&PathSet,int count,Node ** ItemPointers)
{ 
	dbg(Item);
	Node *TempHeader[N]={NULL};
	Node *ItemList=ItemPointers[Item];
	vector<pair<int,int> >temp[N];
	int path=0;
	int Basecount=0;
	while(ItemList!=NULL)
	{
		 Node *K=ItemList;
		  Basecount=0;
		 if(K!=NULL)
		 {
		 	Basecount=K->freq;
		 	K=K->parent;
		 }
		  
		 while(K!=NULL && K->item!=0)
		 {
		 	 
		 	temp[path].push_back({K->item,Basecount});
		 	K=K->parent;

		 }

		 reverse(temp[path].begin(),temp[path].end());
		 for(int i=0;i<temp[path].size();i++)
		 	cout<<temp[path][i].first<<" "<<temp[path][i].second<<" \n";
		 ItemList=ItemList->next;
		  
		 path++;
	}

  
	dbg(path);
	if(path==1)
	{

		GenerateSets(temp[0],PathSet,count);
		// set<int>s;
		// for(int i=0;i<temp[0].size();i++)
		// {
		// 	s.insert(temp[0][i].first);
			 
		// }
		// for(int i=0;i<PathSet.size();i++)
		// {
		// 	s.insert(PathSet[i]);
			 
		// }
		 
		// GlobalSet.insert({s,Basecount});
		return;
	}
	else
	{
		 
		Node *TempRoot=new Node(0);
		int Hash[N]={0};
		for(int i=0;i<N;i++)
		{
			createReducedTree(TempRoot,temp[i],TempHeader,Hash);
		}
		PruneTree(TempRoot,Hash);
		 
		for(int i=0;i<N;i++)
		{
			if(Hash[i]>0)
			{
				dbg(i);dbg(Hash[i]);
			}
			if(Hash[i]<Support)
			TempHeader[i]=NULL;
		}

		set<int>s;
		for(int i=0;i<PathSet.size();i++)
			s.insert(PathSet[i]);
		if(count>=Support)
			GlobalSet.insert({s,count});

		for(int i=0;i<N;i++)
		{

			if(TempHeader[i]!=NULL)
			{
				dbg(i);
				PathSet.push_back(i);
		 		MineTree(TempRoot,i,PathSet,Hash[i],TempHeader);
				PathSet.pop_back();
			}
		}
	}



}



 

void inp()
{
	
	string s;
	 
	 
	string st;
	cout<<"No of items : ";
	cin>>NoofItems;
	cout<<NoofItems<<endl;
	cout<<"No of transaction :";
	cin>>NoofTransactions;
	cout<<NoofTransactions<<endl;
	cout<<"Minimum Support :";
	cin>>Support;
	cout<<Support<<endl;

	fstream f1,f2;
	no_i=0;
	f1.open("tmp.txt", ios::in);
	f2.open("p4.txt",ios::out | ios::in | ios::trunc);
	int t=0;
	no_t=1;
	int n,i;
	 
	while(!f1.eof())
	{
		getline(f1,s,'\n');
		n=s.length();
		t=0;
		i=0;
		while(i<n && s[i]!=' ')
		{
			t=t*10+s[i]-'0';
			i++;
		}
		i++;
		while(i<n)
		{
			t=0;
			while(i<n && s[i]!=' ')
			{
				t=t*10+s[i]-'0';
				i++;
			}
			coun[t]++;
			no_i=max(no_i,t);
			v[no_t].push_back(t);
			i++;
		}
		no_t++;
	}
	no_t-=1;

	 
	// for(int i=1;i<N;i++)
	// {
	// 	if(v[i].size()==0)
	// 	continue;
	// 	cout<<i<<": ";
	// 	for(int j=0;j<v[i].size();j++)
	// 	cout<<v[i][j]<<" ";
	// 	cout<<endl;
	// }
	// cout<<endl;
	
}

int main()
{
	
	inp();
	for(int i=0;i<N;i++)
	{
		sort(v[i].begin(),v[i].end(),comp2);
		if(v[i].size()>0)
		{
			cout<<i<<" : ";
			for(int j=0;j<v[i].size();j++)
			cout<<v[i][j]<<" ";
			cout<<endl;
			
		}
	}
	
	 Node *head=new Node(0);
	 int Hash[N]={0};
	 Node* ItemPointers[N]={NULL};
	
	for(int i=0;i<N;i++)
	{
		if(v[i].size()>0)
			createTree(head,v[i],ItemPointers,Hash);
	}

	vector<int>PathSet;
	for(int i=0;i<N;i++)
	{
		if(Hash[i]>=Support)
		{
			PathSet.push_back(i);
			MineTree(head,i,PathSet,Hash[i],ItemPointers);
			PathSet.pop_back();
		}
	}
	cout<<GlobalSet.size()<<endl;
	for(it=GlobalSet.begin();it!=GlobalSet.end();it++)
	{
		cout<<"Count "<<(*it).second<<"        ";
		for(jt=(*it).first.begin();jt!=(*it).first.end();jt++)
			cout<<*jt<<" ";
		cout<<endl;
		
	}
	 
	
	 
}
