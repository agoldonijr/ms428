##Trabalho de MS428 - Implementação do Simplex

Para compilar em um sistema MacOS use o script bash compile.bash
Para compilar em um sistema Linux (Debian Like) use o script bash compileLinux.bash


## Resolução de sistemas lineares
Para resolver sistemas lineares estamos usando a lib GSL(http://www.gnu.org/software/gsl/).

Aqui temos um exemplo de como usa-lá para resolver um SL: https://www.gnu.org/software/gsl/manual/html_node/Linear-Algebra-Examples.html#Linear-Algebra-Examples


Para instalar a lib no MacOS: `brew install gsl`

Para instalar a lib no Debian/Ubuntu: apt-get install libgsl-dev liblapack-dev

```./compile.bash && ./simplexExec < teste1.in```
