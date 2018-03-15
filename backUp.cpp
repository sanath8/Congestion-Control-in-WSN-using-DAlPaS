#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#define FALSE 0
#define TRUE 1
#define MAX_NODES 30
using namespace std;
//defining the different data structures needed
typedef struct N
{
	int vertex;
	struct N *next;
}node;
typedef node *nodeptr;

typedef struct
{
	int neighbourNode;
	int level;
	int flag;
}neighbourValues;

typedef struct
{
	neighbourValues neighbour[MAX_NODES];

}neighbourTable;

typedef struct
{
	int sendRate;
	int receiveRate;

}transmissionRates;


//Global variables used for simulation


//class containing all the methods for simulation
class networkSimulator{
	
private:
	neighbourTable nTable[MAX_NODES];

	transmissionRates tRates[MAX_NODES];

	int level[MAX_NODES];

	int nTableSize[MAX_NODES];

	nodeptr graph[MAX_NODES];

	int q[MAX_NODES];

	int i,max,visited[MAX_NODES],front,rear;

public:	
	networkSimulator()
	{

		for(int j=0;j<MAX_NODES;j++)
		{
			level[j]=0;
			nTableSize[j]=0;
			q[j]=0;
			visited[j]=0;
		}
		i=0;
		front=0;
		rear=0;
			

	}
	
	void addq(int v)
	{
		q[rear++]=v;
	}


	int deleteq()
	{
		return(q[front++]);
	}


	void insert(int vi,int vj)
	{
		node *p,*q;
		//acquire memory for the new node
		q=(node *)malloc(sizeof(node));
		q->vertex=vj;
		q->next=NULL;
		//insert the node in the linked list for the vertex no. vi
		if(graph[vi]==NULL)
			graph[vi]=q;
		else
		{
		// go to the end of linked list
			p=graph[vi];
			while(p->next!=NULL)
				p=p->next;
			p->next=q;
		}
	}
	int readgraph()
	{  
		int i,vi,vj,no_of_edges,n;
		printf("\nEnter no. of vertices :");
		scanf("%d",&n);
		//initialise G[] with NULL
		for(i=0;i<n;i++)
			graph[i]=NULL;
		//read edges and insert them in G[]
		printf("\nEnter no of edges :");
		scanf("%d",&no_of_edges);
		for(i=0;i<no_of_edges;i++)
		{
			printf("\nEnter an edge (u,v)  :");
			scanf("%d%d",&vi,&vj);
			insert(vi,vj);
			insert(vj,vi);
		}
		return n;
	}
	void displayGraph(int n)
	{
		nodeptr p;
		int i;
		for(i=0;i<n;i++)
		{

			p=graph[i];
			while(p->next!=NULL)
			{
				if(i<p->vertex)
					printf("%d<--->%d\n",i,p->vertex);
				p=p->next;
			}

			if(i<p->vertex)
				printf("%d<--->%d\n",i,p->vertex);

		}
	

	}
	//Running BFS to find level of each node
	void BFS(int v)
	{
		nodeptr temp;
		printf("%d ",v);
		visited[v]=TRUE;
		front=0;
		rear=0;
		addq(v);
		while(front<=rear)
		{
			v=deleteq();
			for(temp=graph[v];temp;temp=temp->next)
				if(!visited[temp->vertex])
				{
					printf("%d ",temp->vertex);
					addq(temp->vertex);
					visited[temp->vertex]=TRUE;
					level[temp->vertex]=level[v]+1;
				}
		}
	}

	void prepareNeighbourTable(int n)
	{
		nodeptr p;	
		int i,j,neighbourCounter;
		for(i=0;i<n;i++)
		{
			neighbourCounter=0;
			p=graph[i];
			while(p->next!=NULL)
			{
				nTable[i].neighbour[neighbourCounter].neighbourNode=p->vertex;
				nTable[i].neighbour[neighbourCounter].level=level[p->vertex];
				nTable[i].neighbour[neighbourCounter++].flag=TRUE;
				p=p->next;
			}
				nTable[i].neighbour[neighbourCounter].neighbourNode=p->vertex;
				nTable[i].neighbour[neighbourCounter].level=level[p->vertex];
				nTable[i].neighbour[neighbourCounter++].flag=TRUE;
				nTableSize[i]=neighbourCounter;


		}
	}

	void displayNeighbourTable(int n)
	{

		int i,j;
		for(i=0;i<n;i++)
		{
			printf("\nDisplaying table %d\n",i);
			printf("Neighbour\tLevel\tFlag\n");
			for(j=0;j<nTableSize[i];j++)
				printf("\n%d\t\t%d\t%d",nTable[i].neighbour[j].neighbourNode,nTable[i].neighbour[j].level,nTable[i].neighbour[j].flag);


		}


	}


	void getTransmissionRates(int n,int destNode)
	{
		int i;
		printf("Enter transmission rates");
		for(i=0;i<n;i++)
		{
			if(i!=destNode)
			{
				printf("node %d",i);
				scanf("%d%d",&tRates[i].sendRate,&tRates[i].receiveRate);
			}
			else
			{
				tRates[i].sendRate=10000;
				tRates[i].receiveRate=0;
				

			}
		
		}

	}

	void calculateLevels(int destNode,int n)
	{
		int i=0;
		level[destNode]=0;
		BFS(destNode);
		printf("\nnode::level\n");
		for(i=0;i<n;i++)
		{
			printf("%d::%d\n",i,level[i]);
		}
		prepareNeighbourTable(n);
	
	
	}

	void sortNeighbourTable(int node)
	{
		neighbourValues tempTableRow;
		int i,j;
		for(i=0;i<nTableSize[node];i++)
		{
			for(j=0;j<nTableSize[node]-i-1;j++)
			{
				if(nTable[node].neighbour[j].level>nTable[node].neighbour[j+1].level)
				{
					tempTableRow=nTable[node].neighbour[j];
					nTable[node].neighbour[j]=nTable[node].neighbour[j+1];
					nTable[node].neighbour[j+1]=tempTableRow;


				}



			}

		}




	}

	void updateNeighbourTable(int node,int neighbour)
	{
		int i;
		for(i=0;i<nTableSize[node];i++)
		{
			if(nTable[node].neighbour[i].neighbourNode==neighbour)
			{
				if(nTable[node].neighbour[i].flag==TRUE)
					nTable[node].neighbour[i].flag=FALSE;
				//else
				//	nTable[node].neighbour[i].flag=TRUE;
				break;

			}

		}

	}

	void flagDecisionAlgo(int n)
	{
		int i;
		nodeptr p;
		for(i=0;i<n;i++)
		{
			if(tRates[i].sendRate<tRates[i].receiveRate)
			{
	
				p=graph[i];
				while(p->next!=NULL)
				{
					updateNeighbourTable(p->vertex,i);
					p=p->next;
				}
					updateNeighbourTable(p->vertex,i);


			}

		}

	}


	void runSimulation(int sourceNode,int destNode,int n)
	{
		int currentNode=sourceNode;
		int bestNode,i;
		flagDecisionAlgo(n);
		printf("\n%d",sourceNode);
		while(currentNode!=destNode)
		{
			sortNeighbourTable(currentNode);
			for(i=0;i<nTableSize[currentNode];i++)
			{
				if(nTable[currentNode].neighbour[i].flag==TRUE && level[currentNode]>nTable[currentNode].neighbour[i].level)
				{
					bestNode=nTable[currentNode].neighbour[i].neighbourNode;
					break;
				}
		
			}
			printf("-->%d",bestNode);
			currentNode=bestNode;
	


		}

	}

};

int main()
{
	networkSimulator nSimulator;
	int i,op,n,destNode,sourceNode;
	do
	{
		printf("\n\n1)Create\n2)Display graph\n3)Display neighbour table\n4)Simulation\n5)Quit");
		printf("\nEnter Your Choice: ");
		scanf("%d",&op);
		switch(op)
		{ 
			case 1: n=nSimulator.readgraph();
				printf("Enter destination node\n");
				scanf("%d",&destNode);
				nSimulator.calculateLevels(destNode,n);
				nSimulator.getTransmissionRates(n,destNode);		
				break;
			case 2: nSimulator.displayGraph(n); 
				break;
			case 3: nSimulator.displayNeighbourTable(n);
				break;
			case 4: printf("Enter source node\n");
				scanf("%d",&sourceNode);
				nSimulator.runSimulation(sourceNode,destNode,n);
				break;
		}
	}while(op!=5);

	return 0;
}
