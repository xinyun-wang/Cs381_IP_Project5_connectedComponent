#include <iostream>
#include <fstream>
#include <string>
using namespace std;

class ImageCompression {
public:
	int numR;
	int numC;
	int minV;
	int maxV;
	int newMinV;
	int newMaxV;
	int* nAry;
	int** ZFAry;
	int** skeletonAry;

	void setZero(int** ary) {
		for (int i = 0; i < numR + 2; i++) {
			for (int j = 0; j < numC + 2; j++) {
				ary[i][j] = 0;
			}
		}
	}

	void loadImage(ifstream& in, int** a) {
		for (int i = 1; i < numR + 1; i++) {
			for (int j = 1; j < numC + 1; j++) {
				in >> a[i][j];
			}
		}
	}


	void loadSkeleton(ifstream& in) {
		int i, j, s;

		while (in >> i >> j >> s) {
			ZFAry[i][j] = s;
		}
	}

	int findMax(int x, int y) {
		int m = 0;
		for (int i = x - 1; i <= x + 1; i++) {
			for (int j = y - 1; j <= y + 1; j++) {
				if (i >= 0 && i <= numR + 1 && j >= 0 && j <= numC + 1 && !(i == x && j == y) && m < max(m, ZFAry[i][j])) {
					m = max(m, ZFAry[i][j]);
				}
			}
		}

		return m;
	}

	bool isLocalMaxima(int i, int j) {
		int m = findMax(i, j);
		return ZFAry[i][j] >= m;
	}

	void computeLocalMaxima() {
		for (int i = 1; i < numR + 1; i++) {
			for (int j = 1; j < numC + 1; j++) {
				if (ZFAry[i][j] > 0 && isLocalMaxima(i, j))
				{
					skeletonAry[i][j] = ZFAry[i][j];
				}
				else
					skeletonAry[i][j] = 0;
			}
		}
	}

	void extractSkeleton(ofstream& out) {
		for (int i = 0; i < numR + 2; i++) {
			for (int j = 0; j < numC + 2; j++) {
				if (skeletonAry[i][j] > 0) {
					out << i << " " << j << " " << skeletonAry[i][j] << endl;
				}
			}
		}
	}

	void threshold(ofstream& out) {
		for (int i = 1; i < numR + 1; i++) {
			for (int j = 1; j < numC + 1; j++) {
				if (ZFAry[i][j] >= 1)
					out << 1 << " ";
				else
					out << 0 << " ";
			}
			out << endl;
		}
	}

	void imgReformat(int** inAry, ofstream& out) {
		string s = to_string(maxV);

		int w = s.length();
		int r = 1;
		while (r < numR + 1) {
			int c = 1;
			while (c < numC + 1) {
				out << inAry[r][c];
				s = to_string(inAry[r][c]);
				int ww = s.length();
				while (ww <= w) {
					out << " ";
					ww++;
				}
				c++;
			}
			out << endl;
			r++;
		}
	}

	void Distance8(ofstream& out, ofstream& de) {
		de << "Entering Distance8() method." << endl;
		distance8pass1(de);
		out << "\n1st pass Distance8" << endl;
		out << numR << " " << numC << " " << newMinV << " " << newMaxV << endl;
		imgReformat(ZFAry, out);

		distance8pass2(de);
		out << "\n2nd pass Distance8" << endl;
		out << numR << " " << numC << " " << newMinV << " " << newMaxV << endl;
		imgReformat(ZFAry, out);
		de << "leaving Distance8() method." << endl;
	}

	void imgaeCompression(ofstream& skeleF, ofstream& out, ofstream& de) {
		de << "Entering imageCompression() method" << endl;
		computeLocalMaxima();
		out << "\nLocal Maxima: SkeletonAry" << endl;
		out << numR << " " << numC << " " << newMinV << " " << newMaxV << endl;
		imgReformat(skeletonAry, out);
		extractSkeleton(skeleF);
		de << "leaving imageCompression() method." << endl;
	}

	void imgaeDeCompression(ofstream& out, ofstream& de) {
		de << "Entering imageDeCompression() method" << endl;
		expansionPass1(de);
		out << "\n1st pass expansion" << endl;
		out << numR << " " << numC << " " << newMinV << " " << newMaxV << endl;
		imgReformat(ZFAry, out);
		expansionPass2(de);
		out << "\n2nd pass expansion" << endl;
		out << numR << " " << numC << " " << newMinV << " " << newMaxV << endl;
		imgReformat(ZFAry, out);
		de << "leaving imageDeCompression() method." << endl;
	}

	void distance8pass1(ofstream& de) {
		newMaxV = 0;
		de << "Entering distance8pass1() method." << endl;
		for (int i = 1; i < numR + 1; i++) {
			for (int j = 1; j < numC + 1; j++) {
				if (ZFAry[i][j] > 0) {
					int m = min(ZFAry[i - 1][j - 1], ZFAry[i - 1][j]);
					m = min(m, ZFAry[i - 1][j + 1]);
					m = min(m, ZFAry[i][j - 1]);
					ZFAry[i][j] = m + 1;
					if (ZFAry[i][j] > newMaxV)
						newMaxV = ZFAry[i][j];
				}
			}
		}
		de << "levaing distance8pass1() method." << endl;
	}

	void distance8pass2(ofstream& de) {
		newMaxV = 0;
		de << "Entering distance8pass1() method." << endl;
		for (int i = numR; i > 0; i--) {
			for (int j = numC; j > 0; j--) {
				if (ZFAry[i][j] > 0) {
					int m = min(ZFAry[i + 1][j + 1], ZFAry[i + 1][j]);
					m = min(m, ZFAry[i + 1][j - 1]);
					m = min(m, ZFAry[i][j + 1]);
					ZFAry[i][j] = min(m + 1, ZFAry[i][j]);
					if (ZFAry[i][j] > newMaxV)
						newMaxV = ZFAry[i][j];
				}
			}
		}
		de << "levaing distance8pass2() method." << endl;
	}

	void expansionPass1(ofstream& de) {
		de << "Entering expansionPass1() method." << endl;
		for (int i = 1; i < numR + 1; i++) {
			for (int j = 1; j < numC + 1; j++) {

				if (ZFAry[i][j] == 0) {
					int m = findMax(i, j) - 1;
					ZFAry[i][j] = max(m, ZFAry[i][j]);
				}
			}
		}
		de << "levaing expansionPass1() method." << endl;
	}

	void expansionPass2(ofstream& de) {
		de << "Entering expansionPass2() method." << endl;
		for (int i = numR; i > 0; i--) {
			for (int j = numC; j > 0; j--) {
				int m = max(ZFAry[i][j], findMax(i, j) - 1);

				if (ZFAry[i][j] < m) {
					ZFAry[i][j] = m;
				}
			}
		}
		de << "levaing expansionPass2() method." << endl;
	}
};

int main(int argc, char* argv[]) {
	ifstream in(argv[1]);
	ofstream out(argv[2]);
	ofstream debug(argv[3]);

	ImageCompression i;
	in >> i.numR >> i.numC >> i.minV >> i.maxV;
	i.newMaxV = 0;
	i.newMinV = 0;

	i.ZFAry = new int* [i.numR + 2];
	i.skeletonAry = new int* [i.numR + 2];

	for (int j = 0; j < i.numR + 2; j++) {
		i.ZFAry[j] = new int[i.numC + 2];
		i.skeletonAry[j] = new int[i.numC + 2];
	}

	string skeletonFileName = (string)argv[1] + "_skeleton.txt";
	ofstream skeletonFile(skeletonFileName);
	string deCompressedFileName = (string)argv[1] + "_deCompressed.txt";
	ofstream deCompressedFile(deCompressedFileName);

	i.setZero(i.ZFAry);
	i.setZero(i.skeletonAry);

	i.loadImage(in, i.ZFAry);

	i.Distance8(out, debug);
	i.imgaeCompression(skeletonFile, out, debug);
	skeletonFile.close();

	ifstream skeletonFile1(skeletonFileName);
	i.setZero(i.ZFAry);
	i.loadSkeleton(skeletonFile1);

	i.imgaeDeCompression(out, debug);
	deCompressedFile << i.numR << " " << i.numC << " " << i.minV << " " << i.maxV << endl;
	i.threshold(deCompressedFile);

	deCompressedFile.close();
	skeletonFile1.close();
	out.close();
	debug.close();
	in.close();
}