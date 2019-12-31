#include<iostream>
#include<cstdlib>
#include<fstream>
#include<queue>
#include<algorithm>
using namespace std;

int N,K, w, s, c, key[1001];

typedef struct Event{
	int keyNum;
	int time;
	int borr;
}Event;

bool cmp(const Event& a, const Event &b)
{
	if(a.time != b.time)
		return a.time < b.time;
	else if(a.borr != b.borr)
		return a.borr < b.borr;
	else
		return a.keyNum < b.keyNum;
}

int ChangeKey(int keyNum, int borrow){
	if(borrow == 1){
		for(int i=1; i<=N; i++){
			if(key[i] == keyNum){
				key[i] = 0;
				break;
			}
		}
	}
	else{
		for(int i=1; i<=N; i++){
			if(key[i] == 0){
				key[i] = keyNum;
				break;
			}
		}
	}
	return 1;
}

int main()
{
	ifstream dataFile("data.txt");
	if(!dataFile.is_open()){
		cout << "Error opening file"; exit (1);
	}
	dataFile>>N>>K;
	//初始化key
	for(int i=1; i<=N; i++){
		key[i] = i;
	} 
	vector<Event> v;
	Event t;
	for(int i=1; i<=K; i++){
		dataFile>>w>>s>>c;
		//事件借 
		t.keyNum = w;
		t.time = s;
		t.borr = 1;
		v.push_back(t);
		//事件还
		t.time = s+c;
		t.borr = 0;
		v.push_back(t); 
	}
	sort(v.begin(),v.end(),cmp);
	for(int i=0; i<v.size(); i++){
		ChangeKey(v[i].keyNum, v[i].borr);
		for(int i=1; i<=N; i++){
			if(key[i] == 0)
				cout<<"X"<<" ";
			else
				cout<<key[i]<<" ";
		}
		cout<<endl;
	}
	return 0;
} 
