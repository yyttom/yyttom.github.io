#include<bits/stdc++.h>
#define rint register int
#define maxn 100005
#define ll long long
using namespace std;
const int bfsize=1<<20;
char bf[bfsize];
char *itail=bf+bfsize;
char *ihead=itail;
char nxtChar(){
	if(ihead==itail)fread(bf,1,bfsize,stdin),ihead=bf;
	return *ihead++;
}
template<typename T> void Read(T &r){
	r=0;int sig=1;char c;
	while((c=nxtChar())<'0' || c>'9')if(c=='-')sig=-1;
	while(c>='0' && c<='9')r=r*10+c-'0',c=nxtChar();
	r*=sig;
}
int N,M,R,P,val[maxn];

//ǰ����
int head[maxn],newp=0;
struct edges{
	int to,nxt;
}e[maxn<<1]; 
void doAdd(int u,int v){
	e[++newp].to=v;
	e[newp].nxt=head[u];
	head[u]=newp;
}

//����
int depth[maxn],fa[maxn],size[maxn],son[maxn],top[maxn];//��ȣ����ף�������С���ض��� 
int seg[maxn],rev[maxn];//�ڵ㵽�߶����ڵ��ӳ�� 
void dfs1(int u,int f){
	depth[u]=depth[f]+1;//��� 
	fa[u]=f;//���ڵ� 
	size[u]=1;// 
	for(int i=head[u];i;i=e[i].nxt){
		int v=e[i].to;
		if(v==f)continue;
		dfs1(v,u);
		size[u]+=size[v];
		if(size[v]>size[son[u]])son[u]=v;
	}
} 
void dfs2(int u,int t){//t��ǰ�������� 
	top[u]=t;
	seg[u]=++seg[0];
	rev[seg[u]]=u; 
	if(!son[u])return;//û���ض���
	dfs2(son[u],t);//�������ض��� 
	for(int i=head[u];i;i=e[i].nxt){//����� 
		int v=e[i].to;
		if(v!=fa[u] && v!=son[u]/*�����*/)dfs2(v,v); //v���µ����������� 
	}	 
} 

//�߶�������
ll sum[maxn<<2],lazy[maxn<<2];//�ı��ռ� 
void pushup(int k){
	sum[k]=(sum[k*2]+sum[k*2+1])%P;
}
void pushdown(int k,int l,int r){
	//�´���ǣ�ά����������
	if(!lazy[k])return;
	int mid=(l+r)>>1,lc=k*2,rc=k*2+1;
	sum[lc]=(sum[lc]+(mid-l+1)*lazy[k]%P)%P;
	sum[rc]=(sum[rc]+(r-mid)*lazy[k]%P)%P;
	lazy[lc]=(lazy[lc]+lazy[k])%P;//ע�����´����ۼƣ����Ǹ��ǣ���Ϊ���ӽڵ㱾��ı�ǿ��ܻ�δ�´� 
	lazy[rc]=(lazy[rc]+lazy[k])%P;
	lazy[k]=0;
}
void doBuild(int k,int l,int r){
	if(l==r){
		sum[k]=val[rev[l]]%P;//ǧ��Ҫ���˷��䣬��ԭ���еĽڵ� 
		return ;
	}
	int mid=(l+r)>>1;
	doBuild(k*2,l,mid);
	doBuild(k*2+1,mid+1,r);
	pushup(k); 
} 
void doUpdate(int k,int l,int r,int L,int R,ll x){
	if(l>R || r<L)return;
	if(L<=l && R>=r){//�޸������������l,r������ 
		sum[k]=(sum[k]+(r-l+1)*x%P)%P; 
		lazy[k]=(lazy[k]+x)%P;
		return;
	}
	pushdown(k,l,r);//�´� 
	int mid=(l+r)>>1;
	doUpdate(k*2,l,mid,L,R,x);
	doUpdate(k*2+1,mid+1,r,L,R,x);
	pushup(k); 
}

ll doQuery(int k,int l,int r,int L,int R){
	if(l>R || r<L)return 0L;
	if(L<=l && R>=r){//��ѯ�����������l,r 
		return sum[k]%P;		
	}
	pushdown(k,l,r);//�´� 
	int mid=(l+r)>>1;
	return (doQuery(k*2,l,mid,L,R)+doQuery(k*2+1,mid+1,r,L,R))%P;
}

//�ʷ�
ll doSplit(int u,int v,int w,int op){
	ll res=0;
	int topu=top[u],topv=top[v];
	while(topu!=topv){
		if(depth[topu]<depth[topv]){ //��top�������Ϊu������ 
			swap(topu,topv);
			swap(u,v);
		}
		//����u��topu���·����һ����������
		if(op==1)doUpdate(1,1,seg[0],seg[topu],seg[u],w);                             
		else if(op==2)res=(res+doQuery(1,1,seg[0],seg[topu],seg[u]))%P;
		//uֱ�Ӵ�top������ߵ����ϲ�����
		u=fa[topu];
		topu=top[u];
	} 
	//u,v��һ����������ǳ�ľ���lca,u-v·������ 
	if(depth[u]>depth[v])swap(u,v);
	//����u��topu���·����һ����������
	if(op==1)doUpdate(1,1,seg[0],seg[u],seg[v],w);                             
	else if(op==2)res=(res+doQuery(1,1,seg[0],seg[u],seg[v]))%P; 
	return res%P;	
} 
int main(){
	freopen("P3384_2.in","r",stdin);
	freopen("test.out","w",stdout); 
	Read(N);Read(M);Read(R);Read(P);
	for(rint i=1;i<=N;++i)Read(val[i]);
	int u,v,w,op;
	for(rint i=1;i<N;++i){
		Read(u);Read(v);
		doAdd(u,v);
		doAdd(v,u);
	}
	dfs1(R,0);
	dfs2(R,R);
	doBuild(1,1,seg[0]);
	for(rint i=1;i<=M;++i){
		Read(op);Read(u);
		if(op==1){
			Read(v);Read(w);
			doSplit(u,v,w,1);
		}else if(op==2){
			Read(v);
			printf("%lld\n",doSplit(u,v,0,2));
		}else if(op==3){
			Read(w);
			doUpdate(1,1,seg[0],seg[u],seg[u]+size[u]-1,w);
		}else if(op==4){
			printf("%lld\n",doQuery(1,1,seg[0],seg[u],seg[u]+size[u]-1));
		}
	}
	return 0;
} 

