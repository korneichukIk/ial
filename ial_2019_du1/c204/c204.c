
/* ******************************* c204.c *********************************** */
/*  Předmět: Algoritmy (IAL) - FIT VUT v Brně                                 */
/*  Úkol: c204 - Převod infixového výrazu na postfixový (s využitím c202)     */
/*  Referenční implementace: Petr Přikryl, listopad 1994                      */
/*  Přepis do jazyka C: Lukáš Maršík, prosinec 2012                           */
/*  Upravil: Kamil Jeřábek, září 2019                                         */
/* ************************************************************************** */
/*
** Implementujte proceduru pro převod infixového zápisu matematického
** výrazu do postfixového tvaru. Pro převod využijte zásobník (tStack),
** který byl implementován v rámci příkladu c202. Bez správného vyřešení
** příkladu c202 se o řešení tohoto příkladu nepokoušejte.
**
** Implementujte následující funkci:
**
**    infix2postfix .... konverzní funkce pro převod infixového výrazu
**                       na postfixový
**
** Pro lepší přehlednost kódu implementujte následující pomocné funkce:
**
**    untilLeftPar .... vyprázdnění zásobníku až po levou závorku
**    doOperation .... zpracování operátoru konvertovaného výrazu
**
** Své řešení účelně komentujte.
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako
** procedury (v jazyce C procedurám odpovídají funkce vracející typ void).
**
**/

#include "c204.h"
int solved;

/*
** Pomocná funkce untilLeftPar.
** Slouží k vyprázdnění zásobníku až po levou závorku, přičemž levá závorka
** bude také odstraněna. Pokud je zásobník prázdný, provádění funkce se ukončí.
**
** Operátory odstraňované ze zásobníku postupně vkládejte do výstupního pole
** znaků postExpr. Délka převedeného výrazu a též ukazatel na první volné
** místo, na které se má zapisovat, představuje parametr postLen.
**
** Aby se minimalizoval počet přístupů ke struktuře zásobníku, můžete zde
** nadeklarovat a používat pomocnou proměnnou typu char.
*/
void untilLeftPar ( tStack* s, char* postExpr, unsigned* postLen ) {
  char c;

  while (!stackEmpty(s) && (c = s->arr[s->top]) != '(') {
    postExpr[(*postLen)++] = c;
    stackPop(s);
  }

  // remove
  if (c == '(') { // may be mistake; TODO: delete this comment if it passes tests
    stackPop(s);
  }
}

/*
** Pomocná funkce doOperation.
** Zpracuje operátor, který je předán parametrem c po načtení znaku ze
** vstupního pole znaků.
**
** Dle priority předaného operátoru a případně priority operátoru na
** vrcholu zásobníku rozhodneme o dalším postupu. Délka převedeného
** výrazu a taktéž ukazatel na první volné místo, do kterého se má zapisovat,
** představuje parametr postLen, výstupním polem znaků je opět postExpr.
*/
void doOperation ( tStack* s, char c, char* postExpr, unsigned* postLen ) {
  char s_top;
  int s_top_prec = 0, c_prec = 0;

  // get given symbol precedence
  if (c == '+' || c == '-') {
    c_prec = 1;
  }else{
    c_prec = 2;
  }

  // parse stack
  while (!stackEmpty(s)) {
    // get stack top item
    stackTop(s, &s_top);

    // if stack top item is opening bracket
    if (s_top == '(') {
      break;
    }

    // get stack top item precedence
    if (s_top == '+' || s_top == '-') {
      s_top_prec = 1;
    }else{
      s_top_prec = 2;
    }

    // if given operator precedence less then one on the stack top
    if (c_prec <= s_top_prec) {
      postExpr[(*postLen)++] = s_top;
      stackPop(s);
    }
    else{
      break;
    }
  }

  // if stack is empty
  stackPush(s, c);
}

/*
** Konverzní funkce infix2postfix.
** Čte infixový výraz ze vstupního řetězce infExpr a generuje
** odpovídající postfixový výraz do výstupního řetězce (postup převodu
** hledejte v přednáškách nebo ve studijní opoře). Paměť pro výstupní řetězec
** (o velikosti MAX_LEN) je třeba alokovat. Volající funkce, tedy
** příjemce konvertovaného řetězce, zajistí korektní uvolnění zde alokované
** paměti.
**
** Tvar výrazu:
** 1. Výraz obsahuje operátory + - * / ve významu sčítání, odčítání,
**    násobení a dělení. Sčítání má stejnou prioritu jako odčítání,
**    násobení má stejnou prioritu jako dělení. Priorita násobení je
**    větší než priorita sčítání. Všechny operátory jsou binární
**    (neuvažujte unární mínus).
**
** 2. Hodnoty ve výrazu jsou reprezentovány jednoznakovými identifikátory
**    a číslicemi - 0..9, a..z, A..Z (velikost písmen se rozlišuje).
**
** 3. Ve výrazu může být použit předem neurčený počet dvojic kulatých
**    závorek. Uvažujte, že vstupní výraz je zapsán správně (neošetřujte
**    chybné zadání výrazu).
**
** 4. Každý korektně zapsaný výraz (infixový i postfixový) musí být uzavřen
**    ukončovacím znakem '='.
**
** 5. Při stejné prioritě operátorů se výraz vyhodnocuje zleva doprava.
**
** Poznámky k implementaci
** -----------------------
** Jako zásobník použijte zásobník znaků tStack implementovaný v příkladu c202.
** Pro práci se zásobníkem pak používejte výhradně operace z jeho rozhraní.
**
** Při implementaci využijte pomocné funkce untilLeftPar a doOperation.
**
** Řetězcem (infixového a postfixového výrazu) je zde myšleno pole znaků typu
** char, jenž je korektně ukončeno nulovým znakem dle zvyklostí jazyka C.
**
** Na vstupu očekávejte pouze korektně zapsané a ukončené výrazy. Jejich délka
** nepřesáhne MAX_LEN-1 (MAX_LEN i s nulovým znakem) a tedy i výsledný výraz
** by se měl vejít do alokovaného pole. Po alokaci dynamické paměti si vždycky
** ověřte, že se alokace skutečně zdrařila. V případě chyby alokace vraťte namísto
** řetězce konstantu NULL.
*/
char* infix2postfix (const char* infExpr) {
  // stack initialization
  tStack stack;
  stackInit(&stack);

  // result string init
  char * post_expr = malloc(MAX_LEN * sizeof(char));
  if (!post_expr) {
    return NULL;
  }

  unsigned post_len = 0; // index for adding symbols to postfix result string

  // start parsing
  for (char c = *infExpr; c != '\0'; c = *(++infExpr)) {
    // if operand
    if ((c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) {
      // add 'c' to result string
      post_expr[post_len++] = c;
    }
    // if opening bracket
    else if (c == '(') {
      // add to stack
      stackPush(&stack, c);
    }
    // if closing bracket
    else if (c == ')') {
      untilLeftPar(&stack, post_expr, &post_len);
    }
    // if operator
    else if (c == '+' || c == '-' || c == '*' || c == '/') {
      doOperation(&stack, c, post_expr, &post_len);
    }
    // if equation ends
    else if (c == '=') {
      while(!stackEmpty(&stack)){
        // adding symbols from stack to result string and removing them from stack
        stackTop(&stack, &(post_expr[post_len++]));
        stackPop(&stack);
      }
      // put '=' to the end of result string
      post_expr[post_len++] = '=';
      break;
    }
  }
  // put EOL to the end of result string
  post_expr[post_len++] = '\0';
  return post_expr;
}

/* Konec c204.c */
