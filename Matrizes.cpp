#include <iostream>
using namespace std;

class Matriz {
protected:
    int lins;
    int cols;
    float** dados;

public:
    //Construtor
    Matriz(int lins, int cols, float* array = nullptr) : lins(lins), cols(cols) {
        dados = new float*[lins];
        for (int i = 0; i < lins; ++i) {
            dados[i] = new float[cols];
            for (int j = 0; j < cols; ++j) {
                dados[i][j] = array[i * cols + j];
            }
        }
    };

    //Destrutor
    ~Matriz() {
        for (int i = 0; i < lins; ++i) {
            delete[] dados[i];
        }
        delete[] dados;
    }

    //Transposta
    Matriz transpor() const {
        float* arrayDadosTra = new float[lins * cols];
        for (int i = 0; i < lins; ++i) {
            for (int j = 0; j < cols; ++j) {
                arrayDadosTra[j * lins + i] = dados[i][j];
            }
        }
        Matriz saida(cols, lins, arrayDadosTra);
        delete[] arrayDadosTra;
        cout << "Transposta:" << endl;
        saida.printMatriz();
        return saida;
    }

    //Adição
    Matriz operator+(const Matriz& parc) const {
        if (lins != parc.lins || cols != parc.cols) {
            cerr << "Matrices of different sizes!" << endl;
            return Matriz(0, 0, nullptr);
        }
        float* arrayDadosSom = new float[lins * cols];

        for (int i = 0; i < lins; ++i) {
            for (int j = 0; j < cols; ++j) {
                arrayDadosSom[i * cols + j] = dados[i][j] + parc.dados[i][j];
            }
        }
        Matriz saida(lins, cols, arrayDadosSom);
        delete[] arrayDadosSom;
        cout << "Soma:" << endl;
        saida.printMatriz();
        return saida;
    }

    //Subtração
    Matriz operator-(const Matriz& parc) const {
        *this + (parc*-1);
    }

    //Produto Matricial
    Matriz operator*(const Matriz& fat) const {
        if (cols != fat.lins) {
            cerr << "Invalid Matriz dimensions for multiplication!" << endl;
            return Matriz(0, 0, nullptr);
        }

        float* arrayDadosMult = new float[lins * fat.cols];

        for (int i = 0; i < lins; ++i) {
            for (int j = 0; j < fat.cols; ++j) {
                arrayDadosMult[i * fat.cols + j] = 0;
                for (int k = 0; k < cols; ++k) {
                    arrayDadosMult[i * fat.cols + j] += dados[i][k] * fat.dados[k][j];
                }
            }
        }
        Matriz saida(lins, fat.cols, arrayDadosMult);
        delete[] arrayDadosMult;
        cout << "Produto:" << endl;
        saida.printMatriz();
        return saida;
    }

    //Produto por Escalar
    Matriz operator*(float numEsc) const {
        float* arrayDadosEsc = new float[lins * cols];
        for (int i = 0; i < lins; ++i) {
            for (int j = 0; j < cols; ++j) {
                arrayDadosEsc[i * cols + j] = dados[i][j] * numEsc;
            }
        }
        Matriz saida(lins, cols, arrayDadosEsc);
        delete[] arrayDadosEsc;
        cout << "Escalar:" << endl;
        saida.printMatriz();
        return saida;
    }

    //Pinter de Matriz
    void getMatriz() const {
        for (int i = 0; i < lins; ++i) {
            for (int j = 0; j < cols; ++j) {
                cout << dados[i][j] << " ";
            }
            cout << endl;
        }
    }
    void printMatriz() const {
        cout << "Matriz (" << lins << "x" << cols << "):" << endl;
        getMatriz();
    }
};

class MatrizQuad : public Matriz {
public:
    // Construtor
    MatrizQuad(int size, float* array = nullptr) : Matriz(size, size, array) {}

    // Determinante
    float determinante() const {
        if (lins != cols) {
            cout << "Matriz nao quadrada, determinante indisponivel" << endl;
            return 0.0;
        }
        if (lins == 1) {
            return dados[0][0];
        }
        if (lins == 2) {
            return (dados[0][0] * dados[1][1] - dados[0][1] * dados[1][0]);
        }
        // Reducao de tamanho recursivo
        float saida = 0.0;
        for (int i = 0; i < lins; ++i) {
            saida += (i % 2 == 0 ? 1 : -1) * dados[0][i] * reduzMatriz(0, i).determinante();
        }
        return saida;
    }

    //Traco
    float traco() const {
        if (this->lins != this->cols) {
            cout << "Matriz nao quadrada, traco indisponivel" << endl;
            return 0.0;
        }
        float saida = 0.0;
        for (int i = 0; i < lins; ++i) {
            saida += dados[i][i];
        }
        return saida;
    }

private:
    // Funcao de Reducao
    MatrizQuad reduzMatriz(int lin, int col) const {
        float* arrayDadosRed = new float[(lins - 1) * (cols - 1)];
        int linRed = 0;
        for (int i = 0; i < lins; ++i) {
            if (i == lin) {
                continue;
            }
            int colRed = 0;
            for (int j = 0; j < cols; ++j) {
                if (j == col) {
                    continue;
                }
                arrayDadosRed[linRed * (cols - 1) + colRed] = dados[i][j];
                ++colRed;
            }
            ++linRed;
        }
        MatrizQuad minor(lins - 1, arrayDadosRed);
        delete[] arrayDadosRed;
        return minor;
    }
};

class MatrizTriS : public MatrizQuad {
public:
    // Construtor
    MatrizTriS(int size, float* array = nullptr) : MatrizQuad(size) {
        dados = new float*[lins];
        for (int i = 0; i < lins; ++i) {
            dados[i] = new float[cols - i];
            for (int j = 0; j < cols - i; ++j) {
                dados[i][j] = array[i * (cols - i - 1) / 2 + j];
            }
        }
    }

    // Printer Triangular Superior
    void printMatriz() const {
        cout << "Matriz Triangular Superior (" << lins << "x" << cols << "):" << endl;
            for (int i = 0; i < lins; ++i) {
            for (int j = 0; j < i; ++j) {
                cout << 0 << " ";
            }
            for (int j = 0; j < cols - i; ++j) {
                cout << dados[i][j] << " ";
            }
            cout << endl;
        }
    }
};

class MatrizTriI : public MatrizQuad {
public:
    // Construtor
    MatrizTriI(int size, float* array = nullptr) : MatrizQuad(size) {
        dados = new float*[lins];
        for (int i = 0; i < lins; ++i) {
            dados[i] = new float[i];
            for (int j = 0; j < cols - i; ++j) {
                dados[i][j] = array[i * (cols - i - 1) / 2 + j];
            }
        }
    }

    // Printer Triangular Inferior
    void printMatriz() const {
        cout << "Matriz Triangular Superior (" << lins << "x" << cols << "):" << endl;
        for (int i = 0; i < lins; ++i) {
            for (int j = 0; j < i; ++j) {
                cout << dados[i][j] << " ";
            }
            for (int j = 0; j < cols - i; ++j) {
                cout << 0 << " ";
            }
            cout << endl;
        }
    }
};

class MatrizDiag : public MatrizQuad {
public:
    // Construtor
    MatrizDiag(int size, float* array) : MatrizQuad(size) {
        for (int i = 0; i < size; ++i) {
            dados[i][i] = array[i];
        }
    }
};

int main() {
    float arrayDadosA[] = {
        4, 8, 7, 7, 9,
        2, 8, 0, 2, 9,
        2, 9, 0, 2, 8,
        7, 4, 4, 6, 4,
        0, 5, 4, 3, 2
    };
    MatrizQuad MatrizA(5, arrayDadosA);

    float arrayDadosB[] = {
        9, 8, 1, 9, 9,
        5, 0, 2, 9, 1,
        2, 8, 1, 6, 2,
        2, 4, 4, 8, 2,
    };
    Matriz MatrizB(4, 5, arrayDadosB);

    float arrayDadosC[] = {
        8, 6, 8, 2,
        8, 9, 6, 6,
        3, 7, 6, 4,
        2, 3, 7, 0,
        8, 2, 9, 5,
    };
    Matriz MatrizC(5, 4, arrayDadosC);

    float arrayDadosD[] = {
        5, 6, 5, 3, 6,
        7, 2, 1, 3, 7,
        6, 8, 5, 1, 2,
        9, 7, 9, 1, 9
    };
    Matriz MatrizD(4, 5, arrayDadosD);

    float arrayDadosE[] = {
        1, 5, 3, 1, 6,
        7, 9, 3, 9, 3,
        2, 6, 2, 4, 3,
        6, 9, 2, 6, 6
    };
    Matriz MatrizE(4, 5, arrayDadosE);

    float arrayDadosF[] = {
        1, 5, 3, 1, 6,
           9, 3, 9, 3,
              2, 4, 3,
                 6, 6,
                    7
    };
    MatrizTriS MatrizF(5, arrayDadosF);

    MatrizF.printMatriz();

    return 0;
}
