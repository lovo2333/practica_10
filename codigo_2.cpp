#include <iostream>
#include <fstream>
#include <string>

using namespace std;

void mostrarContenidoArchivo(const string& nombreArchivo) {
    ifstream archivo(nombreArchivo);
    if (!archivo) {
        throw runtime_error("El archivo no existe.");
    }

    string linea;
    while (getline(archivo, linea)) {
        cout << linea << endl;
    }

    archivo.close();
}

int divide(int a, int b) {
    if (b == 0) {
        throw runtime_error("No se puede dividir por cero.");
    }

    return a / b;
}

class Persona {
private:
    string nombre;
    int edad;
    string direccion;

public:
    // Constructor
    Persona(const string& nombre = "", int edad = 0, const string& direccion = "")
        : nombre(nombre), edad(edad), direccion(direccion) {}

    // Métodos para establecer y obtener los atributos
    void setNombre(const string& nombre) {
        this->nombre = nombre;
    }

    string getNombre() const {
        return nombre;
    }

    void setEdad(int edad) {
        this->edad = edad;
    }

    int getEdad() const {
        return edad;
    }

    void setDireccion(const string& direccion) {
        this->direccion = direccion;
    }

    string getDireccion() const {
        return direccion;
    }

    // Serialización del objeto en un archivo binario
    void serializar(const string& nombreArchivo) {
        ofstream archivo(nombreArchivo, ios::binary);
        if (!archivo) {
            throw runtime_error("Error al abrir el archivo para escritura.");
        }

        archivo.write(reinterpret_cast<const char*>(this), sizeof(Persona));
        archivo.close();
    }

    // Deserialización del objeto desde un archivo binario
    void deserializar(const string& nombreArchivo) {
        ifstream archivo(nombreArchivo, ios::binary);
        if (!archivo) {
            throw runtime_error("Error al abrir el archivo para lectura.");
        }

        archivo.read(reinterpret_cast<char*>(this), sizeof(Persona));
        archivo.close();
    }

    // Mostrar los atributos por pantalla
    void mostrar() const {
        cout << "Nombre: " << nombre << endl;
        cout << "Edad: " << edad << endl;
        cout << "Dirección: " << direccion << endl;
    }
};

int main() {
    try {
        // Leer el contenido de un archivo
        string nombreArchivo;
        cout << "Ingrese el nombre del archivo: ";
        getline(cin, nombreArchivo);
        mostrarContenidoArchivo(nombreArchivo);

        // División de números enteros y manejo de excepciones
        int a, b;
        cout << "Ingrese dos números enteros: ";
        cin >> a >> b;

        try {
            int resultado = divide(a, b);
            cout << "Resultado de la división: " << resultado << endl;
        }
        catch (const exception& e) {
            cerr << "Error: " << e.what() << endl;
        }

        // Serialización y deserialización de un objeto Persona
        Persona persona("Juan", 30, "Calle Principal");
        string archivoPersona = "persona.bin";
        persona.serializar(archivoPersona);

        Persona personaDeserializada;
        personaDeserializada.deserializar(archivoPersona);

        cout << "Persona deserializada:" << endl;
        personaDeserializada.mostrar();
    }
    catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }

    return 0;
}