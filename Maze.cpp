#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;

int x[4] = { -1,0,0,1 }, y[4] = { 0,-1,1,0 };
//Find the shortest path
vector<pair<int, int>> solve(vector<vector<int>> maze, pair<int, int> a, vector<vector<vector<pair<int, int>>>>& path) {
	queue<pair<int, int>> q;
	q.push(a);
	path[a.first][a.second].push_back(a);
	while (!q.empty()) {
		pair<int, int> cur = q.front();
		q.pop();
		for (int i = 0; i < 4; i++) {
			int ny = cur.first + y[i], nx = cur.second + x[i];
			if (maze[ny][nx] != 0 && path[ny][nx].empty()) { q.push({ ny,nx }); path[ny][nx] = path[cur.first][cur.second]; path[ny][nx].push_back({ ny,nx }); }
			if (maze[ny][nx] == 3) { path[ny][nx].pop_back(); return path[ny][nx]; }
		}
	}
	return {};
}

int main() {
	pair<int, int> a;
	//Insert the maze
	string img_path = "imgs/pixel_maze5.png";
	Mat img = imread(img_path);
	if (img.empty()) {
		cout << "Failed to load the image!";
		return -1;
	}
	//Analyze the maze
	int r = img.rows, c = img.cols;
	vector<vector<int>> maze(r, vector<int>(c));
	for (int i = 0; i < r; i++) {
		for (int j = 0; j < c; j++) {
			Vec3b pv = img.at<Vec3b>(i, j);
			if (pv[0] == 0 && pv[1] == 0 && pv[2] == 255) { maze[i][j] = 2; a = { i,j }; }
			else if (pv[0] == 255 && pv[1] == 0 && pv[2] == 0) maze[i][j] = 3;
			else if (pv[0] == 0 && pv[1] == 0 && pv[2] == 0) maze[i][j] = 0;
			else maze[i][j] = 1;
		}
	}
	vector<vector<vector<pair<int, int>>>> path(r, vector<vector<pair<int, int>>>(c));
	vector<pair<int,int>> ans=solve(maze,a,path);
	///Resize
	Size newSize(500,500);
	Mat imgMaze;
	if (!ans.empty()) {
		//Draw the shortest path
		ans.erase(ans.begin());
		for (auto x : ans) {
			Vec3b& pv = img.at<Vec3b>(x.first, x.second);
			pv[0] = 237;
			pv[1] = 43;
			pv[2] = 237;
		}
		resize(img, imgMaze, newSize, 0, 0, INTER_NEAREST);
		imshow("Maze Solver", imgMaze);
		waitKey(0);
	}
	else {
		resize(img, imgMaze, newSize, 0, 0, INTER_NEAREST);
		putText(imgMaze, "NO PATH!", Point(155, 250),FONT_HERSHEY_SIMPLEX, 1.5, Scalar(0, 255, 0), 2);
		imshow("Maze Solver", imgMaze);
		waitKey(0);
	}
	return 0;
}