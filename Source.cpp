#include<iostream>
#include<vector>
#include<cstring>
#include<queue>
#include<list>
#include<concurrent_priority_queue.h>
#define GLUT_DISABLE_ATEXIT_HACK
#include <windows.h>
#include <iostream>
#include <math.h>
#include <map>
#include <GL/glut.h>
//#include <quadTree.h>
#define KEY_ESC 27

#define ll long long
#define ii pair<int,int>
#define INF 1e9
#define sz(x) (int)x.size()
#define forall(it,v) for(auto it = v.begin(); it!=v.end(); ++it) 
#define MAX 250001

GLfloat RED[3] = { 1,0,0 };
GLfloat GREEN[3] = { 0,1,0 };
GLfloat BLUE[3] = { 0,0,1 };
GLfloat WHITE[3] = { 1,1,1 };
GLfloat BLACK[3] = { 0,0,0 };
GLfloat YELLOW[3] = { 1,1,0 };
GLfloat CYAN[3] = { 0,1,1 };
GLfloat MAGENTA[3] = { 1,0,1 };

using namespace std;

class Node {
public:
	int id;
	int x;
	int y;
	vector<pair<int, Node*>> vecinos;
	vector<ii> vecinosSample;
	map<int,bool> indices;
	void addVecino(int w,Node* n2) {
		if (indices[n2->id])return;
		vecinos.push_back({ w,n2 });
		vecinosSample.push_back({ w,n2->id });
		indices[n2->id] = true;
	}
};
double cost(Node* a, Node* b) {
	return sqrt((a->x - b->x) * (a->x - b->x) + (a->y - b->y) * (a->y - b->y));
}
class Graph {
public:
	vector<Node*> nodes;
	Graph(int N):nodes(N*N) {
		int Y = 0;
		int index = 0;
		for (int i = 0; i < N; ++i) {
			int X = 0;
			for (int j = 0; j < N; ++j) {
				nodes[index] = new Node();
				nodes[index]->x = X;
				nodes[index]->y = Y;
				nodes[index]->id = index;
				++index;
				//cout << index << endl;
				++X;
			}
			++Y;
		}
		//cout << "face1 ok\n";
		index = 0;
		for (int i = N+1; i <= ((N*N)-(N+2)); i+=N) {
			for (int j = 0; j < N - 2; ++j) {
				add(i + j, i + j - 1);
				add(i + j, i + j + 1);
				add(i + j, i + j + N);
				add(i + j, i + j + N - 1);
				add(i + j, i + j + N + 1);
				add(i + j, i + j - N);
				add(i + j, i + j - N - 1);
				add(i + j, i + j - N + 1);
			}
		}
		/*for (int i = 0; i < nodes.size(); ++i) {
			cout << nodes[i]->id << " : ";
			forall(it, nodes[i]->vecinos) {
				cout << it->second->id << ' ';
			}
			cout << endl;
		}*/
		for (int i = 0; i < N - 1; ++i) {
			add(i, i + 1);
		}
		for (int i = (N * N) - N; i < (N * N) - 1; ++i) {
			add(i, i + 1);
		}
		for (int i = 0; i <= (N * N) - 2*N; i += N) {
			add(i, i + N);
		}
		for (int i = N - 1; i < (N * N) - 1; i += N) {
			add(i, i + N);
		}
	}
	Node* operator[](int i) {
		return nodes[i];
	}
	void add(int n1, int n2) {
		nodes[n1]->addVecino(cost(nodes[n1],nodes[n2]),nodes[n2]);
		nodes[n2]->addVecino(cost(nodes[n2], nodes[n1]), nodes[n1]);
	}
};
inline double heuristic(Node* a, Node* b) {
	return std::abs(a->x - b->x) + std::abs(a->y - b->y);
}
int N1 = 10;
map<int, bool> lock;
Graph G(N1);
vector<int> path(N1*N1,-1);
int finalpath;
ll dijkstra(int s, int t) {//O(|E| log |V|)
	priority_queue<ii, vector<ii>, greater<ii> > Q;
	vector<ll> dist(N1*N1, INF); vector<int> dad(N1*N1, -1);
	Q.push(make_pair(0, s)); dist[s] = 0;
	while (sz(Q)) {
		ii p = Q.top(); Q.pop();
		if (p.second == t) break;
		forall(it, G[p.second]->vecinos)
			if (dist[p.second] + it->first < dist[it->second->id] && lock[p.second]!=true) {
				dist[it->second->id] = dist[p.second] + it->first + heuristic(G[p.second],it->second);
				dad[it->second->id] = p.second;
				Q.push(make_pair(dist[it->second->id], it->second->id));
			}
	}
	path = dad;
	finalpath = t;
	if (dist[t] < INF)//path generator
		for (int i = t; i != -1; i = dad[i])
			printf("%d%c", i, (i == s ? '\n' : ' '));
	return dist[t];
}


vector<int> v(N1*N1);
bool dfs(int s, int t) {
	finalpath = t;
	if (s == t) { cout<<"llegue\n"; return 1; }
	forall(it, G[s]->vecinos) {
		if (!v[it->second->id]&&lock[it->second->id]!=true) {
			v[it->second->id] = 1;
			path[it->second->id] = s;
			int d = dfs(it->second->id, t);
			if (d)return 1;
		}
	}
	return 0;
}

/*int N;
GRAFO G(0.1,9);
#define add(a,b,w) G[a].push_back({w,b})
ll dijkstra1(int s, int t) {
	priority_queue<ii, vector<ii>, greater<ii>> Q;
	vector<ll> dist(N, INF);
	vector<int> dad(N, -1);
	Q.push({ 0 , s });
	dist[s] = 0;
	while (sz(Q)) {
		ii p = Q.top();
		Q.pop();
		if (p.second == t)break;
		forall(it, G[p.second]->Familia) {
			if (dist[p.second] + cost(G[p.second],*it) < dist[(*it)->getIndex()]) {
				dist[(*it)->getIndex()] = dist[p.second] + cost(G[p.second],*it);
				dad[(*it)->getIndex()] = p.second;
				Q.push({ dist[(*it)->getIndex()],(*it)->getIndex() });
			}
		}
	}
	if (dist[t] < INF) {
		for (int i = t; i != -1; i = dad[i]) {
			printf("%d %c", i, (i == s) ? '\n' : ' ');
		}
	}
	return dist[t];
}
*/
//dibuja un simple gizmo
void displayGizmo()
{
	for (int i = 0; i < G.nodes.size(); ++i) {
		glColor3d(1, 1, 1);
		glBegin(GL_POLYGON);
		//cout << "coordenadas " << G[i]->x << ' ' << G[i]->y << endl;
		glVertex2d(G[i]->x + 0.5, G[i]->y + 0.5);
		glVertex2d(G[i]->x + 0.5, G[i]->y - 0.5);
		glVertex2d(G[i]->x - 0.5, G[i]->y - 0.5);
		glVertex2d(G[i]->x - 0.5, G[i]->y + 0.5);
		glEnd();
	}
	for (int i = finalpath; i != -1; i = path[i]) {
		glColor3d(0, 0, 0);
		glBegin(GL_POLYGON);
		//cout << "coordenadas " << G[i]->x << ' ' << G[i]->y << endl;
		glVertex2d(G[i]->x + 0.5, G[i]->y + 0.5);
		glVertex2d(G[i]->x + 0.5, G[i]->y - 0.5);
		glVertex2d(G[i]->x - 0.5, G[i]->y - 0.5);
		glVertex2d(G[i]->x - 0.5, G[i]->y + 0.5);
		glEnd();
	}
	forall(it, lock) {
		if (it->second) {
			glColor3d(0, 0, 1);
			glBegin(GL_POLYGON);
			//cout << "coordenadas " << G[i]->x << ' ' << G[i]->y << endl;
			glVertex2d(G[it->first]->x + 0.5, G[it->first]->y + 0.5);
			glVertex2d(G[it->first]->x + 0.5, G[it->first]->y - 0.5);
			glVertex2d(G[it->first]->x - 0.5, G[it->first]->y - 0.5);
			glVertex2d(G[it->first]->x - 0.5, G[it->first]->y + 0.5);
			glEnd();
		}
	}
	for (int i = 0; i < G.nodes.size(); ++i) {
		glColor3d(0, 1, 0);
		glBegin(GL_POLYGON);
		//cout << "coordenadas " << G[i]->x << ' ' << G[i]->y << endl;
		glVertex2d(G[i]->x + 0.3, G[i]->y + 0.3);
		glVertex2d(G[i]->x + 0.3, G[i]->y - 0.3);
		glVertex2d(G[i]->x - 0.3, G[i]->y - 0.3);
		glVertex2d(G[i]->x - 0.3, G[i]->y + 0.3);
		glEnd();
	}
	for (int i = finalpath; path[i] != -1 ; i=path[i]) {
		glColor3d(1, 1, 0);
		glBegin(GL_LINES);
		glVertex2d(G[i]->x, G[i]->y);
		glVertex2d(G[path[i]]->x, G[path[i]]->y);
		glEnd();
	}
}


void OnMouseClick(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		//convertir x,y 
		//insertar un nuevo punto en el quadtree
	}
}

void OnMouseMotion(int x, int y)
{
	//opcional 
	//hacer algo x,z cuando se mueve el mouse
}



void idle() { // AGREGAR ESTA FUNCION
	glutPostRedisplay();
}

//funcion llamada a cada imagen
void glPaint(void) {
	//El fondo de la escena al color initial
	glClear(GL_COLOR_BUFFER_BIT); //CAMBIO
	glLoadIdentity();
	glPushMatrix(); // save the current matrix
	glScalef(30, 30, 30); // scale the matrix
	glOrtho(-300.0f, 300.0f, -300.0f, 300.0f, -1.0f, 1.0f);
	//dibujar quadTree (qt->draw())

	//dibuja el gizmo
	displayGizmo();
	glPopMatrix(); // load the unscaled matrix
	//doble buffer, mantener esta instruccion al fin de la funcion
	glutSwapBuffers();
}

//
//inicializacion de OpenGL
//
void init_GL(void) {
	//Color del fondo de la escena
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f); //(R, G, B, transparencia) en este caso un fondo negro

	//modo projeccion
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
}

//en el caso que la ventana cambie de tamaño
GLvoid window_redraw(GLsizei width, GLsizei height) {
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();


}

GLvoid window_key(unsigned char key, int x, int y) {
	switch (key) {
	case KEY_ESC:
		exit(0);
		break;

	default:
		break;
	}

}
//
//el programa principal
//
int main(int argc, char** argv) {
	int s, f;
	cout << "En que nodo inicia : ";
	cin >> s;
	cout << "En que nodo finaliza : ";
	cin >> f;
	int nnn = 0;
	cout << "Cuantos nodos quiere blockear : ";
	cin >> nnn;
	int blockk;
	while (nnn) {
		cin >> blockk;
		lock[blockk] = true;
		--nnn;
	}
	dijkstra(2, 99);
	//dfs(s, f);
	for (int i = finalpath; i != -1; i = path[i])
		printf("%d%c", i, (i == 2 ? '\n' : ' '));
	cout << "Se termino exitosamente\n";
	//Inicializacion de la GLUT
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(600, 600); //tamaño de la ventana
	glutInitWindowPosition(100, 100); //posicion de la ventana
	glutCreateWindow("TP2 bis OpenGL : Bresenham"); //titulo de la ventana

	init_GL(); //funcion de inicializacion de OpenGL

	glutDisplayFunc(glPaint);
	glutReshapeFunc(&window_redraw);
	// Callback del teclado
	glutKeyboardFunc(&window_key);
	glutMouseFunc(&OnMouseClick);
	glutMotionFunc(&OnMouseMotion);
	glutIdleFunc(&idle);


	//qt = new quadTree();
	glutMainLoop(); //bucle de rendering
	//no escribir nada abajo de mainloop
	/*for (int i = 0; i < G.nodes.size(); ++i) {
		cout << G[i]->id << " : ";
		forall(it, G[i]->vecinos) {
			cout << it->second->id << ' ';
		}
		cout << endl;
	}*/
	return 0;
}