# Covid-IUL
A plataforma Covid-IUL destina-se à marcação e efetuação da vacinação de cidadãos para a Covid-19. Na Plataforma Covid-IUL, existem os seguintes conceitos:

• Cidadãos: a pessoa que precisa de ser vacinada. O cidadão tem os seguintes dados:
nome, número de utente, idade, localidade, número de telemóvel, estado da vacinação
(0 – não vacinado, 1 - 1ª inoculação, 2 - 2ª inoculação);

• Enfermeiros: a pessoa que efetua a marcação e vacinação dos cidadãos. Um enfermeiro
tem os seguintes dados – Nome, número de cédula profissional, Centro de Saúde
associado (pela localidade), número de vacinações para covid efetuadas, disponibilidade
(0 – indisponível, 1 – disponível).

• Agenda de vacinação: Agendamento do dia da vacinação, número e nome do utente a
ser vacinado e se é a 1ª ou 2ª inoculação, e nome e número da cédula do enfermeiro que
vai efetuar a vacinação.

Em termos gerais, quando chega um lote de X vacinas, o primeiro enfermeiro disponível trata de
contactar X cidadãos (não vacinados) para agendar a vacinação contra a covid-19. Para isso,
acede à lista de cidadãos registados no centro de saúde onde trabalha e envia uma mensagem
com a marcação da data de vacinação a cada um dos cidadãos, registando esses dados no ficheiro
“agenda.txt”
