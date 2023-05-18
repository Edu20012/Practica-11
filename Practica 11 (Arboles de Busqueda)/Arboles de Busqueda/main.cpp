#include <iostream>
#include <string>
#include <stdexcept>

using namespace std;

template <class T>
class ArbolBusqueda {
private:
    struct Nodo {
        T elem;
        Nodo* izq;
        Nodo* der;

        Nodo(const T& elemento) : elem(elemento), izq(nullptr), der(nullptr) {}
    };

    Nodo* raiz;

    // Funciones auxiliares recursivas
    bool vacio(Nodo* arbol) const {
        return arbol == nullptr;
    }

    T recupera(Nodo* pos) const {
        if (pos == nullptr) {
            throw std::runtime_error("Error de excepción: Insuficiencia de datos");
        }
        return pos->elem;
    }

    void inserta(Nodo*& arbol, const T& elem) {
        if (vacio(arbol)) {
            arbol = new Nodo(elem);
        } else if (elem < arbol->elem) {
            inserta(arbol->izq, elem);
        } else {
            inserta(arbol->der, elem);
        }
    }

    Nodo* localiza(const T& elem, Nodo* arbol) const {
        if (vacio(arbol)) {
            return nullptr;
        } else if (elem == arbol->elem) {
            return arbol;
        } else if (elem < arbol->elem) {
            return localiza(elem, arbol->izq);
        } else {
            return localiza(elem, arbol->der);
        }
    }

    Nodo* menor(Nodo* arbol) const {
        if (vacio(arbol)) {
            return nullptr;
        } else if (vacio(arbol->izq)) {
            return arbol;
        } else {
            return menor(arbol->izq);
        }
    }

    Nodo* mayor(Nodo* arbol) const {
        if (vacio(arbol)) {
            return nullptr;
        } else if (vacio(arbol->der)) {
            return arbol;
        } else {
            return mayor(arbol->der);
        }
    }

    bool es_hoja(Nodo* arbol) const {
        return !vacio(arbol) && vacio(arbol->izq) && vacio(arbol->der);
    }

    void recorrido_preorder(Nodo* arbol) const {
        if (vacio(arbol)) {
            return;
        }
        std::cout << arbol->elem << " ";
        recorrido_preorder(arbol->izq);
        recorrido_preorder(arbol->der);
    }

    void recorrido_inorder(Nodo* arbol) const {
        if (vacio(arbol)) {
            return;
        }
        recorrido_inorder(arbol->izq);
        std::cout << arbol->elem << " ";
        recorrido_inorder(arbol->der);
    }

    void recorrido_postorder(Nodo* arbol) const {
        if (vacio(arbol)) {
            return;
        }
        recorrido_postorder(arbol->izq);
        recorrido_postorder(arbol->der);
        std::cout << arbol->elem << " ";
    }

    void elimina(Nodo*& arbol, Nodo* pos) {
        if (vacio(arbol) || pos == nullptr) {
            throw std::runtime_error("Error de excepción: Insuficiencia de datos");
        }

        if (es_hoja(pos)) {
            delete pos;
            pos = nullptr;
                } else if (vacio(pos->izq)) {
            Nodo* temp = pos;
            pos = pos->der;
            delete temp;
        } else if (vacio(pos->der)) {
            Nodo* temp = pos;
            pos = pos->izq;
            delete temp;
        } else {
            Nodo* pos_reemplazo = mayor(pos->izq);
            pos->elem = pos_reemplazo->elem;
            elimina(pos->izq, pos_reemplazo);
        }
    }

    int altura(Nodo* arbol) const {
        if (vacio(arbol)) {
            return 0;
        }

        int altura_izq = altura(arbol->izq);
        int altura_der = altura(arbol->der);

        return std::max(altura_izq, altura_der) + 1;
    }

    int fact_eq(Nodo* arbol) const {
        if (vacio(arbol)) {
            return 0;
        }

        int altura_der = altura(arbol->der);
        int altura_izq = altura(arbol->izq);

        return altura_der - altura_izq;
    }

    void balanceo(Nodo*& arbol) {
        if (fact_eq(arbol) < -1) {
            if (fact_eq(arbol->izq) == -1) {
                rot_sim_der(arbol);
            } else {
                rot_dob_der(arbol);
            }
        } else if (fact_eq(arbol) > 1) {
            if (fact_eq(arbol->der) == 1) {
                rot_sim_izq(arbol);
            } else {
                rot_dob_izq(arbol);
            }
        }
    }

    void rot_sim_der(Nodo*& arbol) {
        Nodo* aux1 = arbol->izq;
        Nodo* aux2 = aux1->der;
        arbol->izq = aux2;
        aux1->der = arbol;
        arbol = aux1;
    }

    void rot_sim_izq(Nodo*& arbol) {
        Nodo* aux1 = arbol->der;
        Nodo* aux2 = aux1->izq;
        arbol->der = aux2;
        aux1->izq = arbol;
        arbol = aux1;
    }

    void rot_dob_der(Nodo*& arbol) {
        Nodo* aux1 = arbol->izq;
        Nodo* aux2 = aux1->der;
        Nodo* aux3 = aux2->izq;
        Nodo* aux4 = aux2->der;
        arbol->izq = aux4;
        aux1->der = aux3;
        aux2->izq = aux1;
        arbol = aux2;
    }

    void rot_dob_izq(Nodo*& arbol) {
        Nodo* aux1 = arbol->der;
        Nodo* aux2 = aux1->izq;
        Nodo* aux3 = aux2->der;
        Nodo* aux4 = aux2->izq;
        arbol->der = aux4;
        aux1->izq = aux3;
        aux2->der = aux1;
        arbol = aux2;
    }

public:
    ArbolBusqueda() : raiz(nullptr) {}

    ~ArbolBusqueda() {
        destruir_arbol(raiz);
    }

    void inserta(const T& elem) {
        inserta(raiz, elem);
        balanceo(raiz);
    }

    void elimina(const T& elem) {
        Nodo* pos = localiza(elem, raiz);
        elimina(raiz, pos);
        balanceo(raiz);
    }

    bool buscar(const T& elem) const {
        return localiza(elem, raiz) != nullptr;
    }

    void recorridoPreorder() const {
        recorrido_preorder(raiz);
        std::cout << std::endl;
    }

    void recorridoInorder() const {
        recorrido_inorder(raiz);
        std::cout << std::endl;
    }

    void recorridoPostorder() const {
        recorrido_postorder(raiz);
        std::cout << std::endl;
    }

    int alturaArbol() const {
        return altura(raiz);
    }

    bool estaVacio() const {
        return vacio(raiz);
    }

private:
    void destruir_arbol(Nodo* arbol) {
        if (arbol != nullptr) {
            destruir_arbol(arbol->izq);
            destruir_arbol(arbol->der);
            delete arbol;
        }
    }
};

int main() {
    ArbolBusqueda<int> arbol;

    int opcion;
    do {
        std::cout << "Menu de opciones:" << std::endl;
        std::cout << "1. Insertar elemento" << std::endl;
        std::cout << "2. Eliminar elemento" << std::endl;
        std::cout << "3. Buscar elemento" << std::endl;
        std::cout << "4. Recorrido Preorder" << std::endl;
        std::cout << "5. Recorrido Inorder" << std::endl;
        std::cout << "6. Recorrido Postorder" << std::endl;
        std::cout << "7. Altura del arbol" << std::endl;
        std::cout << "8. Salir" << std::endl;
        std::cout << "Ingrese su opcion: ";
        std::cin >> opcion;

        switch (opcion) {
            case 1: {
                int elemento;
                std::cout << "Ingrese el elemento a insertar: ";
                std::cin >> elemento;
                arbol.inserta(elemento);
                std::cout << "Elemento insertado correctamente." << std::endl;
                break;
            }
            case 2: {
                int elemento;
                std::cout << "Ingrese el elemento a eliminar: ";
                std::cin >> elemento;
                arbol.elimina(elemento);
                std::cout << "Elemento eliminado correctamente." << std::endl;
                break;
            }
            case 3: {
                int elemento;
                std::cout << "Ingrese el elemento a buscar: ";
                std::cin >> elemento;
                if (arbol.buscar(elemento)) {
                std::cout << "El elemento " << elemento << " se encuentra en el arbol." << std::endl;
                } else {
                std::cout << "El elemento " << elemento << " no se encuentra en el arbol." << std::endl;
                }
                break;
            }
            case 4: {
                std::cout << "Recorrido Preorder: ";
                arbol.recorridoPreorder();
                break;
            }
            case 5: {
                std::cout << "Recorrido Inorder: ";
                arbol.recorridoInorder();
            break;
            }
            case 6: {
                std::cout << "Recorrido Postorder: ";
                arbol.recorridoPostorder();
            break;
            }
            case 7: {
                std::cout << "Altura del árbol: " << arbol.alturaArbol() << std::endl;
                break;
            }
            case 8: {
                std::cout << "Saliendo del programa..." << std::endl;
                break;
                }
                default: {
                std::cout << "Opcion invalida. Intente nuevamente." << std::endl;
                break;
                }
        }

                 std::cout << std::endl;
        } while (opcion != 8);
        return 0;
}
