#include "tabela_simbolos.h"
#include "ast.h"

PilhaHash *pilha_hash = NULL;
char *ultima_funcao = NULL;
int print_stuff = 0;
e4_check_flag = 1;

/*
TABELA HASH - a tabela hash é pra ser construida utilizando open adressing. 
              ela é um array de estruturas EntradaHash alocado dinamicamente (não confundir com linked lists).
              isso nos dá a opção de expandir o tamanho da tabela se ficar mt densa. é necessário guardar o tamanho do array alocado assim.
CONFLITOS   - o probing em caso de conflito pula pro endereço seguinte. por enquanto ele só dá um pulinho de cada vez. dá pra mudar.
PILHA       - uma estrutura da pilha guarda algumas informações da tabela e a tabela em si. a pilha global é formada por uma linked list.
*/

//#region Auxiliares

//TODO função que retorna o tamanho do tipoSimbolo ISSO AI TA INCOMPLETO!!!! tem q calcular com o tamanho da string tbm
int _tamanho(ValorLexico valor_lexico, TipoSimbolo tipo, int tamanho_vetor) {
    int mult = 1;
    if(tamanho_vetor > 0) mult = tamanho_vetor;
    switch(tipo) {
        case TIPO_INT: return TAMANHO_INT*mult; break;
        case TIPO_FLOAT: return TAMANHO_FLOAT*mult; break;
        case TIPO_STRING: 
            if(tem_valor_string(valor_lexico)) return strlen(valor_lexico.valor_string)*TAMANHO_CHAR; 
            else return 0;
            break;
        case TIPO_BOOL: return TAMANHO_CHAR; break;
        case TIPO_CHAR: return TAMANHO_CHAR*mult; break;
        default: return -1; break;
    }
}

// função que recebe um nome (label?) e uma natureza (variável, função, literal) e cria uma chave dando append nos dois
// exemplo out: func1_n_3
// TODO ver se esse projeto é pra aceitar a declaração do mesmo nome com naturezas diferentes. tipo nome (variavel) != nome(){} (função)
char *_chave(ValorLexico valor_lexico)
{
    // char str[10];
    // sprintf(str, "_n_%d", natureza);

    //return append_str_malloc(nome, str);

    return strdup(valor_lexico.label);
}

char *_chave_label(char *label)
{
    return strdup(label);
}

// função que retorna um valor hash a partir da chave
unsigned long _indice_hash(char *chave)
{
    unsigned char *str = chave;

    // djb2 (1991) por Daniel J. Bernstein, explicação em https://theartincode.stanis.me/008-djb2/
    unsigned long hash = 5381;
    int c;

    while (c = *str++)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}

int _probing(int indice, int tamanho_tabela) {

    indice++;
		
    indice %= tamanho_tabela;

    return indice;
}

// TODO expandir tamanho da tabela quando atinge 70% da capacidade
PilhaHash *_malloc_expande_tabela(PilhaHash *pilha)
{
   return NULL;        
}

Conteudo _novo_conteudo(ValorLexico valor_lexico, Tipo tipo, NaturezaSimbolo natureza, int tamanho_vetor) {
    Conteudo conteudo;
    conteudo.linha = valor_lexico.linha;
    conteudo.coluna = -1; //TODO botar coluna?
    conteudo.tamanho = _tamanho(valor_lexico, tipo, tamanho_vetor);
    conteudo.tipo = tipo;
    conteudo.natureza = natureza;
    conteudo.argumentos = NULL;
    conteudo.valor_lexico = _malloc_copia_vlex(valor_lexico);
    return conteudo;
}

Conteudo _novo_conteudo_literal(ValorLexico valor_lexico, Tipo tipo) {
    Conteudo conteudo;
    conteudo.linha = valor_lexico.linha;
    conteudo.coluna = -1;
    conteudo.tamanho = _tamanho(valor_lexico, tipo, 0);
    conteudo.tipo = tipo;
    conteudo.natureza = NATUREZA_LITERAL;
    conteudo.argumentos = NULL;
    conteudo.valor_lexico = _malloc_copia_vlex(valor_lexico);
    return conteudo;
}

//#endregion Auxiliares

//#region Busca

//  função que retorna uma entrada específica da hash a partir de sua chave
EntradaHash *_busca_pilha(char *chave) {

    PilhaHash *pilha = pilha_hash; 

    EntradaHash *busca = NULL;

    while(pilha != NULL) {

        busca = _busca_topo_pilha(chave, pilha);

        if(busca != NULL) return busca;
        
        pilha = (PilhaHash*)pilha->resto;
    }
	
    return busca;
}

EntradaHash *_busca_topo_pilha(char *chave, PilhaHash *pilha) {

    EntradaHash *tabela = pilha->topo;

    int tamanho_tabela = pilha->tamanho_tabela;

    int indice = _indice_hash(chave) % tamanho_tabela;  

    while(tabela[indice].chave != NULL) {

        if(compare_eq_str(tabela[indice].chave, chave))
            return &tabela[indice]; 
            
        indice = _probing(indice, tamanho_tabela);
    }

    return NULL;
}
//#endregion Busca

//#region Insere

void insere_literal_pilha(TipoSimbolo tipo, ValorLexico valor_lexico) {
    _declara_literal_em_escopo(tipo, valor_lexico);
}

void insere_funcao_pilha(TipoSimbolo tipo, ValorLexico valor_lexico) {

    if(tipo == TIPO_STRING) throwFunctionStringError(valor_lexico.linha, valor_lexico.label);

    EntradaHash *resposta = _declara_em_escopo(NATUREZA_FUNCAO, tipo, valor_lexico, 0);
    ultima_funcao = resposta->conteudo.valor_lexico.label;
}

void libera_ultima_funcao() {
    ultima_funcao = NULL;
}

void insere_variavel_sem_tipo_pilha(ValorLexico valor_lexico) {

    char* chave = _chave(valor_lexico);

    _insere_identificador_sem_tipo_pilha(chave, 0);
    _declara_em_escopo(NATUREZA_VARIAVEL, TIPO_PENDENTE, valor_lexico, 0);
}

void insere_vetor_sem_tipo_pilha(ValorLexico valor_lexico, int tamanho_vetor) {

    char* chave = _chave(valor_lexico);

    _insere_identificador_sem_tipo_pilha(chave, tamanho_vetor);
    _declara_em_escopo(NATUREZA_VETOR, TIPO_PENDENTE, valor_lexico, 0);
}

void _insere_identificador_sem_tipo_pilha(char* chave, int tamanho_vetor) {

    VariavelSemTipoLst *nova_vst;
    nova_vst = malloc(sizeof(VariavelSemTipoLst));
    nova_vst->chave = chave;
    nova_vst->tamanho_vetor = tamanho_vetor;

    if(pilha_hash == NULL) empilha();

    VariavelSemTipoLst *vst_head = pilha_hash->variaveis_sem_tipo;
    nova_vst->proximo = (struct VariavelSemTipoLst *)vst_head;
    pilha_hash->variaveis_sem_tipo = nova_vst;
}

void insere_tipo_identificador_pilha(TipoSimbolo tipo) {
    
    PilhaHash* pilha = pilha_hash;

    if(pilha == NULL) return;

    VariavelSemTipoLst *vst = pilha->variaveis_sem_tipo;

    while(vst != NULL) {
       EntradaHash *busca = _busca_topo_pilha(vst->chave, pilha);

       if(busca != NULL) {

            _verifica_conversao_str(tipo, busca);

           busca->conteudo.tipo = tipo;
           busca->conteudo.tamanho = _tamanho(busca->conteudo.valor_lexico, tipo, vst->tamanho_vetor);
           if(print_stuff) print_pilha();
       } else {
           //TODO help???
            if(print_stuff) printf("nao encontrado?? %s\n", vst->chave);
       }
        VariavelSemTipoLst *antigo_vst = vst;
        vst = (VariavelSemTipoLst *)vst->proximo;
        _libera_head_vst(antigo_vst);
    }

    pilha->variaveis_sem_tipo = vst;
    
    return;
}

void insere_argumento_sem_funcao(TipoSimbolo tipo, ValorLexico valor_lexico) {
    
    _declara_em_escopo(NATUREZA_VARIAVEL, tipo, valor_lexico, 0);

    ArgumentoFuncaoLst *novo_arg_lst;
    novo_arg_lst = malloc(sizeof(ArgumentoFuncaoLst));
    novo_arg_lst->tipo = tipo;

    if(pilha_hash == NULL) {
        printf(">> n ta empilhando antes dos parametros");
        return;
    }

    ArgumentoFuncaoLst *antigo_arg_lst = pilha_hash->argumentos_sem_funcao;
    novo_arg_lst->proximo = (struct ArgumentoFuncaoLst *)antigo_arg_lst;
    pilha_hash->argumentos_sem_funcao = novo_arg_lst;
}

// TODO função que adiciona um argumento à lista de argumentos de uma variável > DO TIPO FUNÇÃO < (checar!!!)
// TODO cara como isso bate com a declaração dos parametros? como se bota uma função na tabela what
// vai ter q ficar com os argumentos pendentes tbm
void adiciona_argumentos_escopo_anterior(Nodo *nodo) {


    PilhaHash *pilha = pilha_hash;
    //TODO tirar prints
    if(pilha == NULL) {
        printf(">> sem pilha? impossive.");
        return;
    }

    if(pilha->argumentos_sem_funcao == NULL) return;

    char *chave_malloc = _chave(nodo->valor_lexico);

    EntradaHash *busca = _busca_topo_pilha(chave_malloc, (PilhaHash*)pilha->resto);

    if(busca != NULL) {
        _verifica_parametros_funcao(pilha->argumentos_sem_funcao, busca);
        
        busca->conteudo.argumentos = reverse_args(pilha->argumentos_sem_funcao);
        pilha->argumentos_sem_funcao = NULL;
    }
    
    free(chave_malloc);
    return;
}

ArgumentoFuncaoLst* reverse_args(ArgumentoFuncaoLst* head) {

    if (head == NULL || head->proximo == NULL)
        return head;

    ArgumentoFuncaoLst* rest = reverse_args((ArgumentoFuncaoLst*)head->proximo);
    ((ArgumentoFuncaoLst*)head->proximo)->proximo = (ArgumentoFuncaoLst*)head;

    head->proximo = NULL;

    return rest;
}

ValorLexico _malloc_copia_vlex(ValorLexico valor_lexico) {

    ValorLexico new_vlex = valor_lexico;
    new_vlex.label = strdup(valor_lexico.label);

    if(tem_valor_string(valor_lexico)) {
        new_vlex.valor_string = strdup(valor_lexico.valor_string);
    }

    return new_vlex;
}

// função que adiciona uma entrada na hash e retorna a recém-adicionada entrada
// chamar ao declarar
// TODO gente como diferenciamos redeclaração e redefinição aqui
/* TODO só procura no topo da pilha, tem que mudar!!!
 -> wait n precisa pq pode declarar algo com msm nome em escopos diferentes 
 -> ta mas e se fizer reatribuição de algo declarado fora do escopo? como achar?
 -> dai tu faz a função de ATRIBUIÇÃO, pq essa aqui tu usa pra DECLARAR.
*/
EntradaHash *_declara_em_escopo(NaturezaSimbolo natureza, TipoSimbolo tipo, ValorLexico valor_lexico, int tamanho_vetor) {

    if(pilha_hash == NULL) empilha();

    PilhaHash* pilha = pilha_hash;

    if(pilha == NULL) return NULL;

    char *chave_malloc = _chave(valor_lexico);

    EntradaHash *busca = _busca_topo_pilha(chave_malloc, pilha);

    if(busca != NULL) {
        //TODO ve redlecaração ou apenas atribuição??
        throwDeclaredError(valor_lexico.linha, valor_lexico.label, busca->conteudo.linha);
        free(chave_malloc);
        return NULL;
    }

    Conteudo conteudo = _novo_conteudo(valor_lexico, tipo, natureza, tamanho_vetor);

    EntradaHash *resposta = _insere_topo_pilha(chave_malloc, pilha, conteudo);

    if(print_stuff) {
        printf("->> OP: DECLARAÇÃO ____________________________________________________________________________________________________\n");
        print_pilha();
    }

    return resposta;
}

// se já existir não insere dnv, mas tbm n dá erro
EntradaHash *_declara_literal_em_escopo(TipoSimbolo tipo, ValorLexico valor_lexico) {

    if(pilha_hash == NULL) empilha();

    PilhaHash* pilha = pilha_hash;

    if(pilha == NULL) return NULL;

    char *chave_malloc = _chave(valor_lexico);

    EntradaHash *busca = _busca_topo_pilha(chave_malloc, pilha);

    if(busca == NULL) {
        Conteudo conteudo = _novo_conteudo_literal(valor_lexico, tipo);
        return _insere_topo_pilha(chave_malloc, pilha, conteudo);
    }

    free(chave_malloc);
    return busca;
}

EntradaHash *_insere_topo_pilha(char *chave, PilhaHash *pilha, Conteudo conteudo) {

    EntradaHash *tabela = pilha->topo;

    if(tabela == NULL) return NULL;

    int tamanho_tabela = pilha->tamanho_tabela;

    int indice = _indice_hash(chave) % tamanho_tabela;  

    while(tabela != NULL) {

        if(tabela[indice].chave == NULL) {

            tabela[indice].chave = chave;
            tabela[indice].conteudo = conteudo;

            pilha->quantidade_atual++; 

            return &tabela[indice]; 
        }

        indice = _probing(indice, tamanho_tabela);
    }
}

// função que "empilha" uma nova hash em cima da atual
void empilha()
{
    PilhaHash *pilha_aux;
    pilha_aux = (PilhaHash*)malloc(sizeof(PilhaHash));

    pilha_aux->quantidade_atual = 0;
    pilha_aux->tamanho_tabela = TAMANHO_INICIAL_HASH;
    pilha_aux->variaveis_sem_tipo = NULL;
    pilha_aux->argumentos_sem_funcao = NULL;

    EntradaHash *tabela = _malloc_tabela();
    pilha_aux->topo = tabela;
    pilha_aux->resto = (struct PilhaHash*)pilha_hash;

    pilha_hash = pilha_aux;

    if(print_stuff) printf("\n->>> empilhando\n");
}

//aloca novo array de EntradaHash (com valores NULL pls)
EntradaHash *_malloc_tabela() {

    EntradaHash *tabela = (EntradaHash*)malloc(sizeof(EntradaHash) * TAMANHO_INICIAL_HASH);

    for (int i = 0; i < TAMANHO_INICIAL_HASH; i++) {
        EntradaHash* entrada = &tabela[i];
        entrada->chave = NULL;
        entrada->conteudo.tipo = -1;
        entrada->conteudo.natureza = -1;
        entrada->conteudo.linha = -1;
        entrada->conteudo.coluna = -1;
        entrada->conteudo.tamanho = -1;
        entrada->conteudo.argumentos = NULL;
    }

    return tabela;
}
//#endregion Insere

//#region Libera 

void libera_pilha() {
    while(pilha_hash != NULL) {
        desempilha();
    }
}

// função que "desempilha" (elimina) a hash e suas informações no topo da pilha
void desempilha()
{
    if(pilha_hash == NULL) return;

    PilhaHash *nova_pilha = (PilhaHash *)pilha_hash->resto;

    PilhaHash *antiga_pilha = pilha_hash;

    _libera_tabela(antiga_pilha->topo, antiga_pilha->tamanho_tabela);

    _libera_vsts(antiga_pilha->variaveis_sem_tipo);

    _libera_argumentos(antiga_pilha->argumentos_sem_funcao);

    free(antiga_pilha);

    pilha_hash = nova_pilha;

    if(print_stuff) printf("\n->>> desempilhando\n");
}

// função que libera a tabela hash e tudo que há dentro dela (i.e. libera a memória caso necessário)
void _libera_tabela(EntradaHash *tabela, int tamanho_tabela) {

    if(tabela == NULL) return;

    for (int i = 0; i < tamanho_tabela; i++) {

        if(tabela[i].chave != NULL) {

            free(tabela[i].chave);

            libera_vlex(tabela[i].conteudo.valor_lexico);

            _libera_argumentos(tabela[i].conteudo.argumentos);
        }
    }

    free(tabela);
}

void _libera_argumentos(ArgumentoFuncaoLst *argumento) {

    if(argumento == NULL) return;

    _libera_argumentos((ArgumentoFuncaoLst *)argumento->proximo);

    free(argumento);
}

void _libera_head_vst(VariavelSemTipoLst *vst) {

    if(vst == NULL) return;

    if(vst->chave != NULL)
        free(vst->chave);

    free(vst);
}

void _libera_vsts(VariavelSemTipoLst *vst) {

    if(vst == NULL) return;

    _libera_vsts((VariavelSemTipoLst *)vst->proximo);

    _libera_head_vst(vst);
}

void _libera_args(ArgumentoFuncaoLst *args) {

    if(args == NULL) return;

    _libera_args((ArgumentoFuncaoLst *)args->proximo);

    free(args);
}

//#endregion Libera 

//#region Verificação

void verifica_atribuicao(Nodo *esq, Nodo *operador, Nodo *dir) {

    if(e4_check_flag) _verifica_conversao_implicita(esq->tipo, esq->valor_lexico, dir->tipo, dir->valor_lexico, 0);

    operador->tipo = esq->tipo = dir->tipo;
}

void verifica_return(Nodo *operador, Nodo *expr1) {

    if(ultima_funcao != NULL) {
        char* chave = _chave_label(ultima_funcao);

        EntradaHash *busca_funcao = _busca_topo_pilha(chave, ((EntradaHash*)pilha_hash->resto));

        free(chave);

        if(busca_funcao != NULL) {
            
            if(busca_funcao->conteudo.tipo != expr1->tipo) {
                if(expr1->tipo == TIPO_CHAR) {
                    throwReturnError(expr1->valor_lexico.linha, expr1->valor_lexico.label);
                }
                if(expr1->tipo == TIPO_STRING) {
                    throwFunctionStringError(expr1->valor_lexico.linha, expr1->valor_lexico.label);
                }
            }
            operador->tipo = busca_funcao->conteudo.tipo;
            return;
        }
    }
    throwReturnError(expr1->valor_lexico.linha, expr1->valor_lexico.label);
}

void verifica_expr_ternaria(Nodo *validacao, Nodo *expr1, Nodo *expr2, Nodo *operador) {

    _verifica_op_str_char_erro(validacao);

    Tipo tipo = get_tipo_inferencia(expr1, expr2);

    printf("\n%i + %i = %i", expr1->tipo, expr2->tipo, tipo);

    operador->tipo = tipo;
}

void verifica_expr_binaria(Nodo *esq, Nodo *operador, Nodo *dir) {

    _verifica_op_str_char_erro(esq);

    _verifica_op_str_char_erro(dir);

    Tipo tipo = get_tipo_inferencia(esq, dir);

    operador->tipo = tipo;
}

void verifica_expr_unaria(Nodo *nodo_unario, Nodo *nodo) {

    if(e4_check_flag) _verifica_op_str_char_erro(nodo);

    if(nodo_unario != NULL)
        nodo_unario->tipo = nodo->tipo;
}

void verifica_input(Nodo *nodo_input, Nodo *nodo) {
    if(nodo->tipo != TIPO_INT && nodo->tipo != TIPO_FLOAT) {
        throwWrongParInput(nodo->valor_lexico.linha, nodo->valor_lexico.label);
    }

    nodo_input->tipo = nodo->tipo;
}

void _verifica_op_str_char_erro(Nodo *nodo) {
    if(nodo->tipo == TIPO_STRING) {
        throwStringToXError(nodo->valor_lexico.linha, nodo->valor_lexico.label, "número");
    }
    if(nodo->tipo == TIPO_CHAR) {
        throwCharToXError(nodo->valor_lexico.linha, nodo->valor_lexico.label, "número");
    }
}

void verifica_output(Nodo *nodo_output, Nodo *nodo) {
    if(nodo->tipo != TIPO_INT && nodo->tipo != TIPO_FLOAT) {
        throwWrongParOutput(nodo->valor_lexico.linha, nodo->valor_lexico.label);
    }

    nodo_output->tipo = nodo->tipo;
}

void verifica_shift(ValorLexico valor_lexico_int) {
    if(valor_lexico_int.valor_int > 16) {
        throwWrongParShift(valor_lexico_int.linha);
    }
}

void inicializacao_nodo(Tipo tipo, Nodo *nodos_inicializados) {

    Nodo *nodo_operacao = nodos_inicializados;
    Nodo *nodo_esq, *nodo_dir;

    while(nodo_operacao != NULL) {

        nodo_esq = (Nodo*)nodo_operacao->filho;

        if(nodo_esq == NULL) break;

        nodo_dir = (Nodo*)nodo_esq->irmao;

        if(nodo_dir == NULL) break;

        if(e4_check_flag) _verifica_conversao_implicita(tipo, nodo_esq->valor_lexico, nodo_dir->tipo, nodo_dir->valor_lexico, 1);

        nodo_esq->tipo = nodo_dir->tipo;   

        nodo_operacao->tipo = tipo;

        nodo_operacao = (Nodo*)nodo_dir->irmao;
    }

}

void _verifica_parametros_funcao(ArgumentoFuncaoLst *parametros, EntradaHash *entrada_funcao) {
    ArgumentoFuncaoLst *aux = (ArgumentoFuncaoLst *)parametros;
    while(aux != NULL) {
        if(aux->tipo == TIPO_STRING) {
            throwFunctionStringError(entrada_funcao->conteudo.linha, "parâmetros");
        }
        aux = (ArgumentoFuncaoLst*)aux->proximo;
    }
}

void _verifica_conversao_implicita(Tipo tipo_esq, ValorLexico esq, Tipo tipo_dir, ValorLexico dir, int inicializacao) {
    if(tipo_esq != tipo_dir) {

        if(tipo_dir == TIPO_STRING) {
            // inicializacao ? 
            // throwWrongTypeError(dir.linha, dir.label, esq.label);
            throwStringToXError(dir.linha, dir.label, esq.label);
        }
        if(tipo_dir == TIPO_CHAR) {
            // inicializacao ?
            // throwWrongTypeError(dir.linha, dir.label, esq.label);
            throwCharToXError(dir.linha, dir.label, esq.label);
        }

        //tive que fazer um compromisso e lançar um erro generico ao inves do requerido em teste abc12.
        if(tipo_dir == TIPO_OUTRO) {
            throwWrongTypeError(dir.linha, dir.label, esq.label);
        }

        if(tipo_esq == TIPO_STRING || tipo_esq == TIPO_CHAR) {
            throwWrongTypeError(dir.linha, dir.label, esq.label);
        }
    }

    if(tipo_dir == TIPO_STRING) {

        char* chave_esq = _chave(esq);
        char* chave_dir = _chave(dir);

        EntradaHash *busca_esq = _busca_pilha(chave_esq);
        EntradaHash *busca_dir = _busca_pilha(chave_dir);

        if(busca_esq != NULL && busca_dir != NULL) {

            //verifica_tamanho_maximo_string(busca_esq, busca_dir);
                
            if(inicializacao) {
                busca_esq->conteudo.tamanho = busca_dir->conteudo.tamanho;
                if(print_stuff) print_pilha();
            }
        }
        free(chave_esq);
        free(chave_dir);
    }
}

void verifica_tamanho_maximo_string(EntradaHash *esq, EntradaHash *dir) {
    if(esq->conteudo.tamanho < dir->conteudo.tamanho) {
        throwStringSizeError(dir->conteudo.valor_lexico.linha, dir->conteudo.valor_lexico.label, esq->conteudo.linha);
    }
}

void _verifica_conversao_str(TipoSimbolo tipo, EntradaHash *entrada) {

    if(tipo == TIPO_STRING && entrada->conteudo.natureza == NATUREZA_VETOR) {
        throwStringVectorError(entrada->conteudo.valor_lexico.linha, entrada->conteudo.valor_lexico.label);
    }
}

void verifica_variavel_no_escopo(Nodo *nodo) {

    ValorLexico valor_lexico = nodo->valor_lexico;

    char* busca_chave = _chave(valor_lexico);

    EntradaHash *busca = _busca_pilha(busca_chave);

    free(busca_chave);

    if(busca == NULL) {
        throwUndeclaredError(valor_lexico.linha, valor_lexico.label);
    }

    if(busca->conteudo.natureza == NATUREZA_VETOR) {
        throwVectorError(valor_lexico.linha, valor_lexico.label, busca->conteudo.linha);
    }

    if(busca->conteudo.natureza == NATUREZA_FUNCAO) {
        throwFunctionError(valor_lexico.linha, valor_lexico.label, busca->conteudo.linha);
    }

    nodo->tipo = busca->conteudo.tipo;
}

void verifica_vetor_no_escopo(Nodo *nodo_vetor, Nodo *nodo_identificador) {

    char* busca_chave = _chave(nodo_identificador->valor_lexico);

    EntradaHash *busca = _busca_pilha(busca_chave);

    free(busca_chave);

    if(busca == NULL) {
        throwUndeclaredError(nodo_identificador->valor_lexico.linha, nodo_identificador->valor_lexico.label);
    }

    if(busca->conteudo.natureza == NATUREZA_VARIAVEL) {
        throwVariableError(nodo_identificador->valor_lexico.linha, nodo_identificador->valor_lexico.label, busca->conteudo.linha);
    }

    if(busca->conteudo.natureza == NATUREZA_FUNCAO) {
        throwFunctionError(nodo_identificador->valor_lexico.linha, nodo_identificador->valor_lexico.label, busca->conteudo.linha);
    }

    nodo_identificador->tipo = nodo_vetor->tipo = busca->conteudo.tipo;
}

void verifica_funcao_no_escopo(ValorLexico valor_lexico, Nodo *args_passados, Nodo *chamada_funcao) {

    char* busca_chave = _chave_label(valor_lexico.label);

    EntradaHash *busca = _busca_pilha(busca_chave);

    free(busca_chave);

    if(busca == NULL) {
        throwUndeclaredError(valor_lexico.linha, valor_lexico.label);
    }

    if(busca->conteudo.natureza == NATUREZA_VARIAVEL) {
        throwVariableError(valor_lexico.linha, valor_lexico.label, busca->conteudo.linha);
    }

    if(busca->conteudo.natureza == NATUREZA_VETOR) {
        throwVectorError(valor_lexico.linha, valor_lexico.label, busca->conteudo.linha);
    }

    int count_args_declarados = _conta_argumentos(busca->conteudo.argumentos);
    int count_args_chamados = _conta_argumentos_nodo(args_passados);

    if(count_args_declarados < count_args_chamados) {
        throwExcessArgsError(valor_lexico.linha, valor_lexico.label, busca->conteudo.linha);

    } else if(count_args_declarados > count_args_chamados) {
        throwMissingArgsError(valor_lexico.linha, valor_lexico.label, busca->conteudo.linha);

    } else {
        _verifica_tipos_argumentos(args_passados, busca->conteudo.argumentos,busca->conteudo.linha);
    }

    chamada_funcao->tipo = busca->conteudo.tipo;
}

void _verifica_tipos_argumentos(Nodo *args_passados, ArgumentoFuncaoLst *args_declarados, int linha_declarada) {

    Nodo *args_passados_aux = args_passados;
    ArgumentoFuncaoLst *args_declarados_aux = (ArgumentoFuncaoLst *)args_declarados;

    while (args_passados_aux != NULL) {

        if(args_passados_aux->tipo != args_declarados_aux->tipo) {
            if(args_passados_aux->tipo == TIPO_CHAR)
                throwWrongTypeArgsError(args_passados_aux->valor_lexico.linha, args_passados_aux->valor_lexico.label, linha_declarada);
            
            if(args_passados_aux->tipo == TIPO_STRING) {
                throwFunctionStringError(args_passados_aux->valor_lexico.linha, args_passados_aux->valor_lexico.label);
            }
        }

        args_passados_aux = (Nodo *)args_passados_aux->filho;
        args_declarados_aux = (ArgumentoFuncaoLst *)args_declarados_aux->proximo;
    }
}
//#endregion Verificação

//#region Prints
void print_pilha() {

    PilhaHash *aux_pilha = pilha_hash;

    int profundidade = 1;

    int total = _conta_tabelas(aux_pilha, 0);

    while(aux_pilha != NULL) {

        int capacidade = aux_pilha->tamanho_tabela;

        printf("\n\n - ESCOPO Nº%02i DA PILHA - | CAPACIDADE: %i | OCUPAÇÃO: %03i\n", (total-profundidade), capacidade, aux_pilha->quantidade_atual);
        char* str = " -------------------------------------------------------------------------------------------------------------------";
        printf("%s\n", str);
        _print_tabela(aux_pilha->topo, capacidade);
        printf("%s\n\n", str);

        profundidade++;

        aux_pilha = (PilhaHash*)aux_pilha->resto;
    }
}

int _conta_tabelas(PilhaHash *pilha, int count) {
    
    PilhaHash *aux_pilha = pilha;

    if(aux_pilha == NULL) return count;

    return _conta_tabelas((PilhaHash*)pilha->resto, ++count);
}

int _conta_argumentos(ArgumentoFuncaoLst *args) {

    int count = 0;

    ArgumentoFuncaoLst *arg_aux = args;

    while (arg_aux != NULL) {
        arg_aux = (ArgumentoFuncaoLst *)arg_aux->proximo;
        count++;
    }
    return count;
}

int _conta_argumentos_nodo(Nodo *nodo) {

    int count = 0;

    Nodo *nodo_aux = nodo;

    while (nodo_aux != NULL) {
        nodo_aux = (Nodo *)nodo_aux->filho;
        count++;
    }
    return count;
}

//printa pilha com tabela e seus valores
void _print_tabela(EntradaHash *tabela, int tamanho) {

    for(int i=0; i < tamanho; i++) {

        char* chave = tabela[i].chave;

        if(chave == NULL) continue;

        Conteudo conteudo = tabela[i].conteudo;

        printf(" | ITEM %03i | NATUREZA: %9s | TIPO: %7s | TAMANHO: %4i | CHAVE: %20s |", 
                i+1, _natureza_str(conteudo.natureza), _tipo_str(conteudo.tipo), conteudo.tamanho, chave
        );

        _print_argumentos(conteudo.argumentos);
    }
}

char* _tipo_str(TipoSimbolo tipo) {
    switch(tipo) {
        case TIPO_INT:      return     "INT"; break;
        case TIPO_FLOAT:    return   "FLOAT"; break;
        case TIPO_BOOL:     return    "BOOL"; break;
        case TIPO_STRING:   return  "STRING"; break;
        case TIPO_CHAR:     return    "CHAR"; break;
        case TIPO_OUTRO:    return   "OUTRO"; break;
        default:            return  "------"; break;
    }
}

char* _natureza_str(NaturezaSimbolo natureza) {
    switch(natureza) {
        case NATUREZA_FUNCAO:   return   "FUNCAO"; break;
        case NATUREZA_LITERAL:  return  "LITERAL"; break;
        case NATUREZA_VARIAVEL: return "VARIAVEL"; break;
        case NATUREZA_VETOR: return "VETOR"; break;
        default:                return "--------"; break;
    }
}

void _print_argumentos(ArgumentoFuncaoLst *argLst) {

    if(argLst != NULL) {
        printf(" ARGS: ");

        ArgumentoFuncaoLst *aux = argLst;

        while(aux != NULL) {
            printf("%s ", _tipo_str(aux->tipo));
            aux = (ArgumentoFuncaoLst *)aux->proximo;
        }

        printf("|");
    }
    printf("\n");
}
//#endregion Prints