# CMake generated Testfile for 
# Source directory: /home/vagrant/latino
# Build directory: /home/vagrant/latino
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
ADD_TEST(test "/home/vagrant/latino/bin/latino" "ejemplos/00-vacio.lat")
ADD_TEST(test1 "/home/vagrant/latino/bin/latino" "ejemplos/01-comentarios.lat")
ADD_TEST(test2 "/home/vagrant/latino/bin/latino" "ejemplos/02-hola.lat")
ADD_TEST(test3 "/home/vagrant/latino/bin/latino" "ejemplos/03-variables.lat")
ADD_TEST(test4 "/home/vagrant/latino/bin/latino" "ejemplos/04-expresiones.lat")
ADD_TEST(test5 "/home/vagrant/latino/bin/latino" "ejemplos/05-si.lat")
ADD_TEST(test6 "/home/vagrant/latino/bin/latino" "ejemplos/06-mientras.lat")
ADD_TEST(test7 "/home/vagrant/latino/bin/latino" "ejemplos/07-hacer.lat")
ADD_TEST(test9 "/home/vagrant/latino/bin/latino" "ejemplos/09-desde.lat")
ADD_TEST(test10 "/home/vagrant/latino/bin/latino" "ejemplos/10-funciones.lat")
ADD_TEST(test11 "/home/vagrant/latino/bin/latino" "ejemplos/11-fibonacci.lat")
ADD_TEST(test12 "/home/vagrant/latino/bin/latino" "ejemplos/12-factorial.lat")
ADD_TEST(test16 "/home/vagrant/latino/bin/latino" "ejemplos/16-funciones_cadena.lat")
ADD_TEST(test17 "/home/vagrant/latino/bin/latino" "ejemplos/17-escribir_archivo.lat")
ADD_TEST(test18 "/home/vagrant/latino/bin/latino" "ejemplos/18-leer_archivo.lat")
ADD_TEST(test19 "/home/vagrant/latino/bin/latino" "ejemplos/19-operadores_logicos.lat")
ADD_TEST(test20 "/home/vagrant/latino/bin/latino" "ejemplos/20-operadores_comparacion.lat")
ADD_TEST(test21 "/home/vagrant/latino/bin/latino" "ejemplos/21-operador_ternario.lat")
SUBDIRS(src)
