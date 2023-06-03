# Mini Foguete

Projeto de desenvolvimento de um Minifoguete com Base em Sistemas de Massa Variável para Materia de Fisica Experimental A.
 
O projeto consiste em uma abordagem sistemática para o desenvolvimento e aprimoramento de foguetes em miniatura, utilizando uma metodologia que envolve aquisição de dados, cálculo de velocidade do foguete e comparação com dados reais obtidos durante os testes com intenção de demonstrar o funcionamento do momento linear em um sistema de massas variáveis. O projeto consiste na obtenção de propelente para realizar teste e através dos dados obtidos calcular velocidade que o foguete pode atingir que após esses será realizado o lançamento de mini foguete e obtenção dos dados de voo para comparação com resultado obtido com cálculo.  

Etapas :
1. Aquisição do Motor. 
2. Teste Estático do Motor
3. Desenvolvimento de uma Avionica para aferição da Altitude.
4. Desenvolvimento do Minifoguete: Utilizando um modelo 3D como base com ajuste para acomodar uma aviônica.
5. Com Massa do Minifoguete Calcular a Velocidade.
6. Lançar o Foguete.
7. Comparar os Dados.

#1. Motor.

Por já ter conhecimento na produção do motor não foi preciso comprar, em que foi produzido KNSu e a carcaça do motor feita com cano de PVC de 3/4.

#2. Teste Estatico 

Para realização do Teste estatico foi utilizada uma celula de carga de 50Kg com Hz711 modificado para 80hz para melhor precisão das leituras obtendo os resultados Abaixo:

#3.Avionica 

Para realizar a aferição da da altitude durante o voo foi produzido uma pcb no eagle com componetes que eu ja possuia sendo eles um ESP8266, BMP280, MPU6050, NRF24L01+, Socket Micro sd, Reguladores de Tensão, Bateria.

#4.Desenvolviemto do Minifoguete 

Foi Utilizado o 3D https://www.thingiverse.com/thing:8754 em que foi modificado acoplamento do motor para comportar outros tamanhos de motores e utilizado o programa openrocket para ajaustar estabilidade do MiniFoguete.

#5.Calculos
