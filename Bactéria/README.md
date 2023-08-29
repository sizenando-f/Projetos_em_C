# Bactérias Contaminadas
"Recentemente contratado pelo ICPC (Instituto Computacional de Pesquisas Científicas), Doutor Henrique
está precisando de sua ajuda novamente! Chefiando um projeto super secreto para o Reino de Sildávia, Dr.
Henrique enfrenta um grande problema. As bactérias envolvidas no projeto estão sendo destruídas após
serem contaminadas por um tipo de vírus muito raro. Este vírus é capaz de destruir qualquer trecho de
código genético das bactérias que seja igual ao seu RNA (código genético do vírus). Sabendo disso, Dr.
Henrique quer criar um tipo de bactéria que seja resistente a este tipo de vírus, porém, quer preservar a
maior quantidade de código genético das bactérias originais"

**Objetivo** <br>
Encontrar a maior substring genética (contém apenas as proteínas A, C, G, T) entre um
grupo de bactérias contaminadas

<details>
<summary>Detalhes de funcionamento:</summary>
<p>Suponhamos que o RNA do vírus é a sequência TCGA. E supomos também que Dr. Henrique possua 3
bactérias de DNA's: </p>
<ul>
  <li>Bactéria 1: GCTTTCGACGAT</li>
  <li>Bactéria 2: GATCGAGCTTCGAA</li>
  <li>Bactéria 3: GGTCTAGCTAAT</li>
</ul>
<p>Após a infecção, o vírus irá procurar o primeiro trecho de código a partir do início do DNA da bactéria que
seja igual ao seu RNA e irá destruí-lo. E irá repetir o processo até que não haja mais código para ser
destruído.</p>
<ul>
  <li>Bactéria 1: GCTT<strong>TCGA</strong>CGAT -> GCT<strong>TCGA</strong>T -> GCTT</li>
  <li>Bactéria 2: GA<strong>TCGA</strong>GCTTCGAA -> GAGCT<strong>TCGA</strong>A -> GAGCTA</li>
  <li>Bactéria 3: GGTCTAGCTAAT</li>
</ul>
  
<p>Agora que todas as bactérias estão infectadas, iremos encontrar a maior substring comum a todas, que
será o DNA da bactéria que Dr. Henrique tanto procura:</p>
<ul>
  <li>Bactéria 1: <strong>GCT</strong>T</li>
  <li>Bactéria 2: GA<strong>GCT</strong>A</li>
  <li>Bactéria 3: GGTCTA<strong>GCT</strong>AAT</li>
</ul>
DNA resultante: GCT
</details>

_O programa é chamado por CMD_
