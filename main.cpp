#include <iostream>
#include <string>
using namespace std;

int key[3][3] = { {17, 17, 5 },
                  {21, 18, 21},
                  { 2, 2, 19}
};
//int key[3][3] = { {6, 24, 1 },
//                  {13, 16, 10},
//                  { 20, 17, 15}
//};
//int key[3][3] = { {0, 11, 15 },
//                  {7, 0, 1},
//                  { 4, 19, 0}
//};

void generate_ciphertext(string plaintext, string& ciphertext) {
    int sum;
    for (int i = 0; i < plaintext.length(); i += 3) {
        for (int j = 0; j < 3; ++j) {
            sum = 0;
            for (int k = 0; k < 3; ++k) {
                sum += key[j][k] * (plaintext[i + k] - 'a');
            }
            ciphertext += (sum % 26 + 'a');
        }
    }
}
void generate_plaintext(string ciphertext, string& plaintext, int** inverse) {
    int sum;
    plaintext.clear();
    plaintext = "";
    for (int i = 0; i < ciphertext.length(); i += 3) {
        for (int j = 0; j < 3; ++j) {
            sum = 0;
            for (int k = 0; k < 3; ++k) {
                sum += inverse[j][k] * (ciphertext[i + k] - 'a');
            }
            plaintext += (sum % 26 + 'a');
        }
    }
}
int getCofactor(int n, int row, int col) {
    int arr[4];
    int index = -1;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (i == row || col == j)continue;
            arr[++index] = key[i][j];
        }
    }
    return (arr[0] * arr[3]) - (arr[1] * arr[2]);
}
void getInverse(int** inverse, int n, int inDeterminant) {
    int sign = 1;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            inverse[j][i] = (sign * getCofactor(n, i, j)) % 26;
            if (inverse[j][i] < 0) inverse[j][i] += 26;
            inverse[j][i] = inverse[j][i] * inDeterminant % 26;
            sign = -sign;
        }
    }
}
int get_inverse_of_determinant(int n) {
    int sum = 0;
    int sign = 1;
    for (int i = 0; i < n; ++i) {
        sum += sign * key[0][i] * getCofactor(n, 0, i);
        sign = -sign;
    }
    sum %= 26;
    if (sum < 0)sum += 26;
    int count = 1;
    while (sum * count % 26 != 1)++count;
    return count;
}
int main() {
    string plaintext = "", ciphertext = "";
    unsigned int redundant = 0;
    int n = 3;
    cout << "Enter the plaintext => ";
    cin >> plaintext;
    while (plaintext.length() % n != 0) {
        plaintext += 'a';
        ++redundant;
    }    
    generate_ciphertext(plaintext, ciphertext);     //generate ciphertext
    cout << "\nciphertext = ";
    for (int i = 0; i < ciphertext.length() - redundant; ++i)cout << ciphertext[i];
    cout << endl;
    int** inverse;
    inverse = new int* [n];
    for (int i = 0; i < n; ++i)inverse[i] = new int[n];
    int inDeterminant = get_inverse_of_determinant(n);  //get inverse of the determinant value
    getInverse(inverse, n, inDeterminant);      //get inverse matrix
    cout << "\ninverse matrix :\n";
    for (int i = 0; i < n; ++i) {
        cout << "\t\t";
        for (int j = 0; j < n; ++j) {            
            cout << inverse[i][j] << '\t';
        }
        cout << endl << endl;
    }
    generate_plaintext(ciphertext, plaintext, inverse);     //generate plaintext
    cout << "plaintext = ";
    for (int i = 0; i < plaintext.length() - redundant; ++i)cout << plaintext[i];
    cout << endl;
}