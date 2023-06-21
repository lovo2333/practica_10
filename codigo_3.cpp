#include <iostream>
#include <fstream>
#include <string>

using namespace std;

class Producto {
public:
    string nombre;
    float precio;
    int cantidad;

    // Constructor
    Producto(const string& nombre = "", float precio = 0.0, int cantidad = 0)
        : nombre(nombre), precio(precio), cantidad(cantidad) {}

    // Función para mostrar los detalles del producto
    void mostrar() const {
        cout << "Nombre: " << nombre << endl;
        cout << "Precio: " << precio << endl;
        cout << "Cantidad disponible: " << cantidad << endl;
    }
};

class Inventario {
private:
    string nombreArchivo;

public:
    // Constructor
    Inventario(const string& nombreArchivo) : nombreArchivo(nombreArchivo) {}

    // Función para agregar un nuevo producto al inventario
    void agregarProducto(const Producto& producto) {
        ofstream archivo(nombreArchivo, ios::app | ios::binary);
        if (!archivo) {
            throw runtime_error("Error al abrir el archivo de inventario para escritura.");
        }
        archivo.write(reinterpret_cast<const char*>(&producto), sizeof(Producto));
        archivo.close();
    }

    // Función para buscar un producto por su nombre en el inventario
    Producto buscarProducto(const string& nombreProducto) {
        ifstream archivo(nombreArchivo, ios::in | ios::binary);
        if (!archivo) {
            throw runtime_error("Error al abrir el archivo de inventario para lectura.");
        }

        Producto producto;
        bool encontrado = false;

        while (archivo.read(reinterpret_cast<char*>(&producto), sizeof(Producto))) {
            if (producto.nombre == nombreProducto) {
                encontrado = true;
                break;
            }
        }

        archivo.close();

        if (!encontrado) {
            throw runtime_error("No se encontró el producto en el inventario.");
        }

        return producto;
    }

    // Función para actualizar la información de un producto en el inventario
    void actualizarProducto(const string& nombreProducto, const Producto& productoNuevo) {
        fstream archivo(nombreArchivo, ios::in | ios::out | ios::binary);
        if (!archivo) {
            throw runtime_error("Error al abrir el archivo de inventario para lectura y escritura.");
        }

        Producto producto;
        bool encontrado = false;

        while (archivo.read(reinterpret_cast<char*>(&producto), sizeof(Producto))) {
            if (producto.nombre == nombreProducto) {
                encontrado = true;
                streampos pos = archivo.tellg();
                archivo.seekp(pos - static_cast<streamoff>(sizeof(Producto)));
                archivo.write(reinterpret_cast<const char*>(&productoNuevo), sizeof(Producto));
                break;
            }
        }

        archivo.close();

        if (!encontrado) {
            throw runtime_error("No se encontró el producto en el inventario.");
        }
    }

    // Función para realizar una venta de un producto
    void venderProducto(const string& nombreProducto, int cantidadVenta) {
        fstream archivo(nombreArchivo, ios::in | ios::out | ios::binary);
        if (!archivo) {
            throw runtime_error("Error al abrir el archivo de inventario para lectura y escritura.");
        }

        Producto producto;
        bool encontrado = false;

        while (archivo.read(reinterpret_cast<char*>(&producto), sizeof(Producto))) {
            if (producto.nombre == nombreProducto) {
                encontrado = true;
                if (cantidadVenta > producto.cantidad) {
                    throw runtime_error("Cantidad disponible insuficiente para realizar la venta.");
                }
                producto.cantidad -= cantidadVenta;
                streampos pos = archivo.tellg();
                archivo.seekp(pos - static_cast<streamoff>(sizeof(Producto)));
                archivo.write(reinterpret_cast<const char*>(&producto), sizeof(Producto));
                break;
            }
        }

        archivo.close();

        if (!encontrado) {
            throw runtime_error("No se encontró el producto en el inventario.");
        }
    }

    // Función para generar un informe de inventario
    void generarInforme() {
        ifstream archivo(nombreArchivo, ios::in | ios::binary);
        if (!archivo) {
            throw runtime_error("Error al abrir el archivo de inventario para lectura.");
        }

        Producto producto;

        while (archivo.read(reinterpret_cast<char*>(&producto), sizeof(Producto))) {
            producto.mostrar();
            cout << endl;
        }

        archivo.close();
    }
};

int main() {
    try {
        Inventario inventario("inventario.dat");

        // Agregar productos al inventario
        Producto producto1("Producto1", 10.5, 50);
        inventario.agregarProducto(producto1);

        Producto producto2("Producto2", 15.75, 100);
        inventario.agregarProducto(producto2);

        // Buscar un producto por su nombre
        string nombreProducto = "Producto1";
        Producto productoEncontrado = inventario.buscarProducto(nombreProducto);
        cout << "Producto encontrado:" << endl;
        productoEncontrado.mostrar();
        cout << endl;

        // Actualizar la información de un producto
        Producto productoNuevo("Producto1", 12.0, 40);
        inventario.actualizarProducto(nombreProducto, productoNuevo);

        // Realizar una venta de un producto
        string nombreProductoVenta = "Producto2";
        int cantidadVenta = 20;
        inventario.venderProducto(nombreProductoVenta, cantidadVenta);

        // Generar un informe de inventario
        inventario.generarInforme();
    }
    catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }

    return 0;
}