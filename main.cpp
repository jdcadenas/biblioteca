/**********************************************************************************
    TRABAJO PRÁCTICO
        ASIGNATURA: PROCESAMIENTO DE DATOS
        CÓDIGO: 330
        NOMBRE DEL ESTUDIANTE: JOSÉ DANIEL CADENAS
        CÉDULA DE IDENTIDAD: 6.816.938
        CORREO ELECTRÓNICO DEL ESTUDIANTE: jdcadenas@gmail.com
        TELÉFONO: 0426-6115101
        CARRERA: 237 MANTENIMIENTO DE SISTEMAS INFORMÁTICOS
        LAPSO ACADÉMICO: 2018 -1
 ***********************************************************************************/


#include <fstream>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <iostream>
using std::cout;
using std::cin;
using std::endl;
using std::fstream;
using std::ofstream;
using std::ifstream;
using std::ios;

class Libro {
private:
  char codigo[20];
  char autor[20];
  char titulo[40];
  int cant_ejemplar_existentes;
  int cant_ejemplar_disponible;
  bool disponible;
public:
  Libro();
  void menu_libro();
  void detalles_libro();
  void mostrar_libro();
  bool buscar_libro_devolucion(char* codigobuscar);
  bool buscar_libro(char* codigobuscar);
  char* titulo_libro_codigo(char* codigobuscado);

  void registrar_libro();
  void borrar_libro();
  void leer_libro();

  void leer_libro_titulo();
  void leer_libro_autor();


};

class Usuario {
private:
  char cedula[10];
  char nombre[20];
  char apellido[20];
  char telefono[20];
  int cant_libros;
  int cant_libros_prestados[3];
  char libros_Prestados[3][40];
public:
  Usuario();
  void menu_usuario();
  void detalles_usuario();
  void mostrar_usuario();
  void buscar_usuario();
  bool buscar_cedula(char* cedulabuscar);
  bool buscar_cedula_devolucion(char* cedulabuscar);
  void registrar_usuario();
  void leer_registro_usuario();
  void borrar_registro_usuario();

};

class Prestamo {
private:
  char codigo_prestamo[20];
  char cedula_usuario_prestamo[10];
  time_t fechar_prestamo;
  time_t fechar_devolucion_prestamo;
public:

  bool detalles_prestamo();
  bool detalles_devolucion();
  void mostrar_prestamo();
  void mostrar_devolucion();
  void mostrar_prestamo_usuario();

  void registrar_prestamo();
  void registrar_devolucion_libro();

  void leer_prestamo();
  void leer_prestamo_estudiante();
  void borrar_registro_prestamo();
};

Libro::Libro() {
  disponible = false;
  cant_ejemplar_existentes = 0;
  cant_ejemplar_disponible = cant_ejemplar_existentes;
}

Usuario::Usuario() {
  for (int i = 0; i < 3; i++) {
    cant_libros_prestados[i] = -1;
    for (int j = 0; j < 40; j++)
      libros_Prestados[i][j] = '\0';
  }
}

void Prestamo::registrar_prestamo() {
  char fechar[25];
  time_t fecha_actual;
  fecha_actual = time(NULL);
  ctime(&fecha_actual);
  strcpy(fechar, ctime(&fecha_actual));
  cout << fechar;

  fstream archivoPrestamo;

  if (!archivoPrestamo) {
    cout << "Error ! Archivo no encontrado!!" << endl;
    return;
  }

  if (detalles_prestamo()) {
    archivoPrestamo.open("registro.prestamo", ios::binary | ios::app);
    fechar_prestamo = time(0);
    fechar_devolucion_prestamo = 0;
    archivoPrestamo.write((char*) (this), sizeof (*this));
    archivoPrestamo.close();
    mostrar_prestamo();
  } else {
    cout << "no se proceso prestamo";
  }
  system("read()");
}

void Prestamo::registrar_devolucion_libro() {
  ofstream archivoPrestamo;
  archivoPrestamo.open("registro.prestamo", ios::binary | ios::app);
  if (!archivoPrestamo) {
    cout << "Error ! Archivo no encontrado!!" << endl;
    return;
  }
  if (detalles_devolucion()) {
    archivoPrestamo.open("registro.prestamo", ios::binary | ios::app);
    fechar_prestamo = 0;
    fechar_devolucion_prestamo = time(0);
    archivoPrestamo.write((char*) (this), sizeof (*this));
    archivoPrestamo.close();
    mostrar_prestamo();
  } else {
    cout << "no se proceso prestamo";
  }
  system("read()");
}

bool Prestamo::detalles_prestamo() {
  Libro L;
  Usuario U;
  char c;
  cin.get(c);
  char opcion;
  bool encontrado = false;
  cout << "\nIntroduzca cedula usuario: ";
  cin.getline(cedula_usuario_prestamo, 10, '\n');
  if (!U.buscar_cedula(cedula_usuario_prestamo)) {
    cout << "\nUsuario rechazado " << endl;
    return false;

  }
  cin.get(c);
  cout << "\nIntroduzca codigo libro: ";
  cin.getline(codigo_prestamo, 20, '\n');
  while ((!L.buscar_libro(codigo_prestamo)) && encontrado == true) {
    cout << "Libro no encontrado " << endl;
    cout << "Desea salir?(S/N)" << endl;
    cin>>opcion;
    if (opcion == 's' || opcion == 'S') {
      return false;
    } else {
      cin.get(c);
      cout << "\nIntroduzca codigo libro: ";
      cin.getline(codigo_prestamo, 20, '\n');
    }

  }
  return true;
}

bool Prestamo::detalles_devolucion() {
  Libro L;
  Usuario U;
  char c;
  cin.get(c);
  char opcion;
  bool encontrado = false;
  cout << "\nIntroduzca cedula usuario: ";
  cin.getline(cedula_usuario_prestamo, 10, '\n');
  if (!U.buscar_cedula_devolucion(cedula_usuario_prestamo)) {
    cout << "\nUsuario recharzado " << endl;
    return false;

  }
  cin.get(c);
  cout << "\nIntroduzca codigo libro: ";
  cin.getline(codigo_prestamo, 20, '\n');
  while ((!L.buscar_libro_devolucion(codigo_prestamo)) && encontrado == true) {
    cout << "Libro no encontrado " << endl;
    cout << "Desea salir?(S/N)" << endl;
    cin>>opcion;
    if (opcion == 's' || opcion == 'S') {
      return false;
    } else {
      cin.get(c);
      cout << "\nIntroduzca codigo libro: ";
      cin.getline(codigo_prestamo, 20, '\n');
    }

  }
  return true;
}

void Prestamo::leer_prestamo() {
  char codigo_Libro[10];
  ifstream archivoEntrada;
  bool encontrado = false;
  archivoEntrada.open("registro.prestamo", ios::binary);
  if (!archivoEntrada) {
    cout << "Error ! Archivo no encontrado!!" << endl;
    return;
  }
  system("clear");
  cout << "\n****Prestamo****" << endl;
  cout << "COLOCAR CODIGO LIBRO : ";
  cin>>codigo_Libro;
  cout << endl;
  cout << "\tCODIGO:  " << "\tEJEMPLAR" << "\tCEDULA:";
  cout << "\tF.PRESTAMO: " << "\tF.DEVOLUCION" << endl;
  cout << "-------------------------------" << endl;
  while (!archivoEntrada.eof()) {
    if (archivoEntrada.read((char *) (this), sizeof (*this))) {
      if (strcmp(this->codigo_prestamo, codigo_Libro) == 0) {
        encontrado = true;
        //cout<<"\nLIBRO ENCONTRADO!\n";
        mostrar_prestamo();

        //	break;
      }

    }
  }
  if (encontrado == false)
    cout << "\nLIBRO NO ENCONTRADO !" << endl;
  archivoEntrada.close();

  system("read()");
}

void Prestamo::leer_prestamo_estudiante() {
  char cedula_usuario[10];
  ifstream archivoEntrada;
  bool encontrado = false;
  archivoEntrada.open("registro.prestamo", ios::binary);
  if (!archivoEntrada) {
    cout << "Error ! Archivo no encontrado!!" << endl;
    return;
  }
  system("clear");
  cout << "\n****Prestamo****" << endl;
  cout << "COLOCAR CEDULA: ";
  cin>>cedula_usuario;
  cout << endl;
  cout << "\tCODIGO:  " << "\tEJEMPLAR";
  cout << "\tF.PRESTAMO: " << "\tF.DEVOLUCION" << endl;
  cout << "---------------------------------------------------------------------" << endl;
  while (!archivoEntrada.eof()) {
    if (archivoEntrada.read((char*) (this), sizeof (*this))) {
      if (strcmp(this->cedula_usuario_prestamo, cedula_usuario) == 0) {
        encontrado = true;
        Libro L;
        cout << "titulo: " << L.titulo_libro_codigo(codigo_prestamo) << endl;
        mostrar_prestamo_usuario();
      }

    }
  }
  if (encontrado == false)
    cout << "\nUSUARIO NO ENCONTRADO !" << endl;
  archivoEntrada.close();

  system("read()");
}

void Prestamo::mostrar_prestamo() {
  cout << "\t" << codigo_prestamo << "\t\t" << cedula_usuario_prestamo;
  cout << "\t" << ctime(&fechar_prestamo) << "\t" << endl;

}

void Prestamo::mostrar_devolucion() {

  cout << "\t" << codigo_prestamo << "\t\t" << cedula_usuario_prestamo;
  cout << "\t" << "\t\t" << ctime(&fechar_devolucion_prestamo) << "\t" << endl;

}

void Prestamo::mostrar_prestamo_usuario() {
  cout << "\t" << codigo_prestamo;
  cout << "\t" << ctime(&fechar_prestamo) << "\t" << ctime(&fechar_devolucion_prestamo) << endl;

}

void Libro::menu_libro() {
  int opcion;
  Prestamo P;
  system("clear");
  cout << "***Menu Biblioteca-Libros***" << endl;
  while (true) {
    cout << "Seleccione una opción " << endl;
    cout << "LIBRO" << endl;
    cout << "\n\t 1-->Registro de Libros";
    cout << "\n\t 2-->Mostrar libros";
    cout << "\n\t 3-->borrar de Libros";

    cout << endl << "PRESTAMO Y DEVOLUCION";

    cout << "\n\t 4-->Registrar prestamo";
    cout << "\n\t 5-->Registrar devolucion";
    cout << "\n\t 6-->Consultar prestamos por Libro";
    cout << "\n\t 7-->Regresar";
    cout << "\nIntroduzca Selección: ";
    cin>>opcion;
    switch (opcion) {
      case 1:
        system("clear");
        registrar_libro();
        break;
      case 2:
        system("clear");
        leer_libro();
        break;
      case 3:
        system("clear");
        borrar_libro();
        break;
      case 4:
        system("clear");
        P.registrar_prestamo();
        break;
      case 5:
        system("clear");
        P.registrar_devolucion_libro();
        break;
      case 6:
        system("clear");
        P.leer_prestamo();
        break;
      default:
        cout << "\nIntroduzca Selección";
        return;
    }
  }
  system("read()");
  system("clear");
  return;
}

void Libro::detalles_libro() {
  disponible = true;
  char c;
  cin.get(c);
  cout << "\nIntroduzca codigo libro: ";
  cin.getline(codigo, 20, '\n');
  cout << "Autor: ";
  cin.getline(autor, 20, '\n');
  cout << "Título: ";
  cin.getline(titulo, 40, '\n');
  cout << "Cantidad de ejemplares existentes:  ";
  cin>>cant_ejemplar_existentes;

  cant_ejemplar_disponible = cant_ejemplar_existentes;
}

void Usuario::detalles_usuario() {
  char c;
  cin.get(c);
  cout << "\nIntroduzca cedula: ";
  cin.getline(cedula, 10, '\n');
  cout << "\nNombre: ";
  cin.getline(nombre, 20, '\n');
  cout << "Apellido: ";
  cin.getline(apellido, 20, '\n');
  cout << "Telefono: ";
  cin.getline(telefono, 20, '\n');
  cant_libros = 0;
}

void Libro::mostrar_libro() {
  cout << "DATOS LIBRO" << endl;
  cout << "Código: " << codigo << endl;
  cout << "Autor: " << autor << endl;
  cout << "Titulo: " << titulo << endl;
  cout << "Cantidad de ejemplares existentes  " << cant_ejemplar_existentes << endl;
  cout << "Cantidad de ejemplares disponibles  " << cant_ejemplar_disponible << endl;

  cout << "-------------------------------" << endl;

}

void Usuario::mostrar_usuario() {
  cout << "DATOS ESTUDIANTE" << endl << endl;
  cout << "Cedula: " << cedula << endl;
  cout << "nombre: " << nombre << endl;
  cout << "Apellido: " << apellido << endl;
  cout << "telefono: " << telefono << endl;
  cout << "numero libros: " << cant_libros << endl;
  cout << "-------------------------------" << endl;

}

void Libro::registrar_libro() {
  char opcion;
  ofstream archivoSalida;
  archivoSalida.open("registro.libros", ios::binary | ios::app);

  if (!archivoSalida) {
    cout << "Error ! Archivo no encontrado!!" << endl;
    return;
  }
  do {
    system("clear");
    detalles_libro();
    archivoSalida.write((char*) (this), sizeof (*this));
    cout << "Desea agregar otro libro?(S/N)" << endl;
    cin>>opcion;
  } while (opcion == 's' || opcion == 'S');
  archivoSalida.close();
}

void Usuario::registrar_usuario() {
  char opcion;
  ofstream archivoSalida;
  archivoSalida.open("registro.usuarios", ios::binary | ios::app);
  if (!archivoSalida) {
    cout << "Error ! Archivo no encontrado!!" << endl;
    return;
  }
  do {
    system("clear");
    detalles_usuario();
    archivoSalida.write((char*) (this), sizeof (*this));
    cout << "Desea agregar otro usuario?(S/N)" << endl;
    cin>>opcion;
  } while (opcion == 's' || opcion == 'S');
  archivoSalida.close();
}

void Libro::leer_libro() {
  ifstream archivoEntrada;
  archivoEntrada.open("registro.libros", ios::binary);
  if (!archivoEntrada) {
    cout << "Error ! Archivo no encontrado!!" << endl;
    return;
  }
  system("clear");
  cout << "\n****Libros Registrados****" << endl;
  int pagina;
  while (!archivoEntrada.eof()) {
    pagina = 0;
    while (pagina < 4 && !archivoEntrada.eof()) {
      if (archivoEntrada.read((char*) (this), sizeof (*this))) {
        mostrar_libro();
        cout << endl;
        pagina++;
      }
    }
    system("clear");
  }
  archivoEntrada.close();

}

void Usuario::leer_registro_usuario() {
  ifstream archivoEntrada;
  archivoEntrada.open("registro.usuarios", ios::binary);
  if (!archivoEntrada) {
    cout << "Error ! Archivo no encontrado!!" << endl;
    return;
  }
  system("clear");
  cout << "\n****Usuarios Registrados****" << endl;
  while (!archivoEntrada.eof()) {
    if (archivoEntrada.read((char*) (this), sizeof (*this))) {
      mostrar_usuario();
    }
  }
  archivoEntrada.close();
  system("read()");
}

void Libro::leer_libro_titulo() {
  char titulo_libro[20];
  char c;
  cin.get(c);
  ifstream archivoEntrada;
  bool encontrado = false;

  archivoEntrada.open("registro.libros", ios::binary);
  if (!archivoEntrada) {
    cout << "Error ! Archivo no encontrado!!" << endl;
    return;
  }

  cout << "\n****Libros Por autor****" << endl;
  cout << "\nIntroduzca titulo libro: ";
  cin.getline(titulo_libro, 20, '\n');

  cout << endl;

  while (!archivoEntrada.eof()) {
    if (archivoEntrada.read((char*) (this), sizeof (*this))) {
      if (strcmp(this->titulo, titulo_libro) == 0) {
        encontrado = true;
        cout << "\nLIBRO ENCONTRADO!\n";
        mostrar_libro();
      }
    }
  }
  if (encontrado == false)
    cout << "\nLIBRO NO ENCONTRADO !" << endl;
  archivoEntrada.close();
}

char* Libro::titulo_libro_codigo(char* codigobuscado) {
  static char* mensaje;
  ifstream archivoEntrada;
  bool encontrado = false;
  archivoEntrada.open("registro.libros", ios::binary);
  while (!archivoEntrada.eof()) {
    if (archivoEntrada.read((char*) (this), sizeof (*this))) {

      if (strcmp(this->codigo, codigobuscado) == 0) {
        encontrado = true;
        archivoEntrada.close();
        mensaje = this->titulo;
        return mensaje;
      }
    }
  }
  if (encontrado == false) {
    archivoEntrada.close();
    mensaje = "no encontrado";
    return mensaje;
  }
}

void Libro::leer_libro_autor() {
  char autor_libro[20];
  char c;
  cin.get(c);

  ifstream archivoEntrada;
  bool encontrado = false;
  archivoEntrada.open("registro.libros", ios::binary);
  if (!archivoEntrada) {
    cout << "Error ! Archivo no encontrado!!" << endl;
    return;
  }
  system("clear");
  cout << "\n****Libros Por autor****" << endl;
  cout << "\nIntroduzca autor libro: ";
  cin.getline(autor_libro, 20, '\n');
  cout << endl;
  while (!archivoEntrada.eof()) {
    if (archivoEntrada.read((char*) (this), sizeof (*this))) {
      if (strcmp(this->autor, autor_libro) == 0) {
        encontrado = true;
        cout << "\nLIBRO ENCONTRADO!\n";
        mostrar_libro();
      }
    }
  }
  if (encontrado == false)
    cout << "\nLIBRO NO ENCONTRADO !" << endl;

  archivoEntrada.close();
  //system("read()");
  system("read()");
}

bool Libro::buscar_libro(char* codigobuscar) {
  bool encontrado;
  char confirmar;
  ifstream archivoEntrada;
  archivoEntrada.open("registro.libros", ios::binary);
  fstream salidaLibro;
  salidaLibro.open("temp.libros", ios::out | ios::app);
  encontrado = false;
  while (!archivoEntrada.eof()) {
    if (archivoEntrada.read((char*) (this), sizeof (*this))) {
      if (strcmp(this->codigo, codigobuscar) == 0) {
        encontrado = true;
        mostrar_libro();
        if (cant_ejemplar_disponible != 0) {
          cout << "Esta seguro de este libro ? (S/N)" << endl;
          cin>>confirmar;
          if (confirmar == 's' || confirmar == 'S') {
            cant_ejemplar_disponible--;
            salidaLibro.write((char*) (this), sizeof (*this));
          } else {
            salidaLibro.write((char*) (this), sizeof (*this));
            break;
          }
        } else {
          cout << "libros no disponible";
          salidaLibro.write((char*) (this), sizeof (*this));
        }
      } else {
        //no es el registro buscado escribo en temporal
        salidaLibro.write((char*) (this), sizeof (*this));
      }

    }//fin de lectura del registro libro

  }//fin de lectura del archivo
  archivoEntrada.close();
  salidaLibro.close();
  remove("registro.libros");
  rename("temp.libros", "registro.libros");
  return encontrado;
}

bool Libro::buscar_libro_devolucion(char* codigobuscar) {
  bool encontrado;
  char confirmar;
  ifstream archivoEntrada;
  archivoEntrada.open("registro.libros", ios::binary);
  fstream salidaLibro;
  salidaLibro.open("temp.libros", ios::out | ios::app);
  encontrado = false;
  while (!archivoEntrada.eof()) {
    if (archivoEntrada.read((char*) (this), sizeof (*this))) {
      if (strcmp(codigo, codigobuscar) == 0) {
        encontrado = true;
        mostrar_libro();
        if (cant_ejemplar_disponible != 0) {
          cout << "Esta seguro de este libro ? (S/N)" << endl;
          cin>>confirmar;
          if (confirmar == 's' || confirmar == 'S') {
            cant_ejemplar_disponible++;
            salidaLibro.write((char*) (this), sizeof (*this));
          } else {
            salidaLibro.write((char*) (this), sizeof (*this));
          }
        } else {
          cout << "libros no disponible";
          salidaLibro.write((char*) (this), sizeof (*this));
        }
      } else {
        //no es el registro buscado escribo en temporal
        salidaLibro.write((char*) (this), sizeof (*this));
      }

    }//fin de lectura del registro libro

  }//fin de lectura del archivo
  archivoEntrada.close();
  salidaLibro.close();
  remove("registro.libros");
  rename("temp.libros", "registro.libros");
  return encontrado;
}

bool Usuario::buscar_cedula(char* cedulabuscar) {

  bool encontrado;
  char confirmar;
  ifstream archivoEntrada;
  archivoEntrada.open("registro.usuarios", ios::binary);
  fstream salidaUsuario;
  salidaUsuario.open("temp.usuarios", ios::out | ios::app);
  encontrado = false;
  while (!archivoEntrada.eof()) {
    if (archivoEntrada.read((char*) (this), sizeof (*this))) {
      if (strcmp(this->cedula, cedulabuscar) == 0) {
        encontrado = true;
        mostrar_usuario();
        if (cant_libros < 3) {
          cout << "Esta seguro de este usuario ? (S/N)" << endl;
          cin>>confirmar;
          if (confirmar == 's' || confirmar == 'S') {
            cant_libros++;
            salidaUsuario.write((char*) (this), sizeof (*this));
            cout << "LIBRO prestado";
          } else {
            salidaUsuario.write((char*) (this), sizeof (*this));
          }
        } else {
          cout << "no puede solicitar mas de tres libros";
          salidaUsuario.write((char*) (this), sizeof (*this));
        }
      } else {
        //no es el registro buscado escribo en temporal
        salidaUsuario.write((char*) (this), sizeof (*this));

      }

    }//fin de la lectura del registro

  }
  archivoEntrada.close();
  salidaUsuario.close();
  remove("registro.usuarios");
  rename("temp.usuarios", "registro.usuarios");

  return encontrado;

}

bool Usuario::buscar_cedula_devolucion(char* cedulabuscar) {

  bool encontrado;
  // char confirmar;
  ifstream archivoEntrada;
  archivoEntrada.open("registro.usuarios", ios::binary);
  fstream salidaUsuario;
  salidaUsuario.open("temp.usuarios", ios::out | ios::app);
  encontrado = false;
  while (!archivoEntrada.eof()) {
    if (archivoEntrada.read((char*) (this), sizeof (*this))) {
      if (strcmp(this->cedula, cedulabuscar) == 0) {
        encontrado = true;
        mostrar_usuario();

        cant_libros--;

        salidaUsuario.write((char*) (this), sizeof (*this));
      } else {
        //no es el registro buscado escribo en temporal
        salidaUsuario.write((char*) (this), sizeof (*this));
      }//fin de la lectura del registro

    }
  }
  archivoEntrada.close();
  salidaUsuario.close();
  remove("registro.usuarios");
  rename("temp.usuarios", "registro.usuarios");

  return encontrado;

}

void Libro::borrar_libro() {
  fstream entradaLibro, salidaLibro;
  entradaLibro.open("registro.libros", ios::in);
  salidaLibro.open("temp.libros", ios::out | ios::app);

  bool encontrado = false;
  char confirmar;
  char codigo_libro[20];
  char c;
  cin.get(c);
  if (!entradaLibro) {
    cout << "Error ! Archivo no encontrado!!" << endl;
    return;
  }

  entradaLibro.seekg(0, ios::end);
  int numregistros = entradaLibro.tellg() / sizeof (*this);
  cout << "\n Hay " << numregistros << " registro en el archivo" << endl;

  system("read()");

  cout << "COLOCAR CODIGO LIBRO : ";
  cin.getline(codigo_libro, 20, '\n');
  cout << endl;
  entradaLibro.seekg(0, ios::beg);
  while (!entradaLibro.eof()) {
    if (entradaLibro.read((char*) (this), sizeof (*this))) {
      if (strcmp(this->codigo, codigo_libro) == 0) {
        encontrado = true;
        cout << "\nLIBRO ENCONTRADO!\n";
        this->mostrar_libro();
        cout << "Esta seguro borrar este libro ? (S/N)" << endl;
        cin>>confirmar;
        if (confirmar == 'n' || confirmar == 'N')
          salidaLibro.write((char*) (this), sizeof (*this));
      } else {
        salidaLibro.write((char*) (this), sizeof (*this));
      }

    }


  }
  if (encontrado == false)
    cout << "Registro no encontrado !! \n";
  entradaLibro.close();
  salidaLibro.close();
  remove("registro.libros");
  rename("temp.libros", "registro.libros");

}

void Usuario::borrar_registro_usuario() {
  fstream entradaUsuario, salidaUsuario;
  entradaUsuario.open("registro.usuarios", ios::in);
  salidaUsuario.open("temp.usuarios", ios::out | ios::app);

  bool encontrado = false;
  char confirmar;
  char cedula_usuario[10];
  if (!entradaUsuario) {
    cout << "Error ! Archivo no encontrado!!" << endl;
    return;
  }

  entradaUsuario.seekg(0, ios::end);
  int numregistros = entradaUsuario.tellg() / sizeof (*this);
  cout << "\n Hay " << numregistros << " registro en el archivo" << endl;
  cout << "COLOCAR CEDULA USUARIO : ";
  cin>>cedula_usuario;
  cout << endl;
  entradaUsuario.seekg(0, ios::beg);
  while (!entradaUsuario.eof()) {
    if (entradaUsuario.read((char*) (this), sizeof (*this))) {
      if (strcmp(this->cedula, cedula_usuario) == 0) {
        encontrado = true;
        cout << "\nLIBRO ENCONTRADO!\n";
        this->mostrar_usuario();
        cout << "Esta seguro borrar este usuario ? (S/N)" << endl;
        cin>>confirmar;
        if (confirmar == 'n' || confirmar == 'N')
          salidaUsuario.write((char*) (this), sizeof (*this));
      } else
        salidaUsuario.write((char*) (this), sizeof (*this));
    }


  }
  if (encontrado == false)
    cout << "Registro no encontrado !! \n";

  entradaUsuario.close();
  salidaUsuario.close();
  remove("registro.usuarios");
  rename("temp.usuarios", "registro.usuarios");
}

void Usuario::menu_usuario() {
  int opcion;
  Libro L;
  Prestamo P;
  system("clear");
  cout << "***Menu Usuario**" << endl;
  while (true) {
    cout << "Seleccione una opción ";
    cout << "LIBRO" << endl;
    cout << "\n\t1-->Registro de Usuarios";
    cout << "\n\t2-->Mostrar Usuarios";
    cout << "\n\t3-->borrar de Usuarios" << endl;
    cout << "CONSULTAS" << endl;
    cout << "\n\t4-->Libros Existentes dado titulo";
    cout << "\n\t5-->Libros dado autor";
    cout << "\n\t6-->Prestamo de un estudiante";
    cout << "\n\t7-->Regresar";
    cout << "\nIntroduzca Selección: ";
    cin>>opcion;
    switch (opcion) {
      case 1:
        system("clear");
        registrar_usuario();
        break;
      case 2:
        system("clear");
        leer_registro_usuario();
        break;
      case 3:
        system("clear");
        borrar_registro_usuario();
        break;
      case 4:
        system("clear");
        L.leer_libro_titulo();
        break;
      case 5:
        system("clear");
        L.leer_libro_autor();
        break;
      case 6:
        system("clear");
        P.leer_prestamo_estudiante();
        break;
      default:
        cout << "\nIntroduzca Selección";
        return;
    }
  }
  system("read()");
  return;

}

int main() {
  Libro L;
  Usuario U;

  int opcion;
  cout << "***Biblioteca Estudiantil***" << endl;
  while (true) {
    cout << "Seleccione una opción ";
    cout << "\n\t1-->Usuario";
    cout << "\n\t2-->Biblioteca";
    cout << "\n\t3-->Salida";
    cout << "\nIntroduzca Selección: ";
    cin >> opcion;
    switch (opcion) {
      case 1:
        U.menu_usuario();
        break;
      case 2:
        L.menu_libro();
        break;
      case 3:
        exit(0);
        break;
      default:
        cout << "\nIntroduzca Selección";
        exit(0);
    }
  }
  system("read()");
  return 0;
}
