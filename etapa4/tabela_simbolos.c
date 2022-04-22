#include "tabela_simbolos.h"

PilhaHash *global_pilha_hash = NULL;
ArgumentoFuncaoLst *global_parametros_sem_funcao = NULL; // para inserir uma lista de parametros nos atributos de uma função ainda não reconhecida pela gramática
VariavelSemTipoLst *global_variaveis_sem_tipo = NULL; // para inserir e atualizar no escopo uma lista de declarações cujo tipo ainda não foi reconhecido pela gramática
char *global_ultima_funcao = NULL; // para buscar a função atual e seu tipo ao analisar o tipo de um retorno
int E4_CHECK_FLAG = 1; // existe pra habilitar/desabilitar as verificações de tipos da E4 e testar outras etapas livremente.

extern int print_simbolos;

/*
TABELA HASH - a tabela hash usa open adressing.
              ela é um array de estruturas EntradaHash alocado dinamicamente (não confundir com linked lists).
              isso nos dá a opção de expandir o tamanho da tabela se ficar mt densa. é necessário guardar o tamanho do array alocado assim.
CONFLITOS   - o probing em caso de conflito pula pro endereço seguinte. por enquanto ele só dá um pulinho de cada vez. dá pra mudar.
PILHA       - uma estrutura da pilha guarda algumas informações da tabela e a tabela em si. a pilha global é formada por uma linked list.
*/

//#region Auxiliares

int _conta_tabelas(PilhaHash *pilha, int count) {
    
    PilhaHash *aux_pilha = pilha;

    if(aux_pilha == NULL) return count;

    return _conta_tabelas(pilha->resto, ++count);
}

int _conta_argumentos(ArgumentoFuncaoLst *args) {

    int count = 0;

    ArgumentoFuncaoLst *arg_aux = args;

    while (arg_aux != NULL) {
        arg_aux = arg_aux->proximo;
        count++;
    }
    return count;
}

int _conta_argumentos_nodo(Nodo *nodo) {

    int count = 0;

    Nodo *nodo_aux = nodo;

    while (nodo_aux != NULL) {
        nodo_aux = nodo_aux->irmao;
        count++;
    }
    return count;
}

//Função que retorna o tamanho do tipoSimbolo
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

/* faz uma cópia do label de valor léxico que deve ser liberado depois (mas não precisa ser assim, pode ser apenas a cópia de um ponteiro). 
é desse jeito para possibilitar a mudança do formato de chaves de alguma forma que fosse possível possuir diferentes entradas hash de mesmo 
nome mas naturezas diferentes (ex: variável e função com mesmo nome no mesmo escopo). */
char *_chave(ValorLexico valor_lexico)
{
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

void _verifica_ocupacao_tabela(PilhaHash *pilha) {
    float percent = (float)pilha->quantidade_atual/(float)pilha->tamanho_tabela;
    if(percent >= .6) 
        _malloc_expande_tabela(pilha);
}

void _malloc_expande_tabela(PilhaHash *pilha) {

    EntradaHash *tabela = pilha->topo;

    int antigo_tamanho = pilha->tamanho_tabela;

    int novo_tamanho = antigo_tamanho + TAMANHO_INICIAL_HASH;

    tabela = (EntradaHash *) realloc(tabela, sizeof(EntradaHash) * (novo_tamanho));

    for (int i = antigo_tamanho; i < novo_tamanho; i++) {
        EntradaHash* entrada = &tabela[i];
        _inicializa_entrada(entrada);
    }

    pilha->topo = tabela;
    pilha->tamanho_tabela = novo_tamanho;
}

Conteudo _novo_conteudo(ValorLexico valor_lexico, Tipo tipo, NaturezaSimbolo natureza, int tamanho_vetor) {
    Conteudo conteudo;
    conteudo.linha = valor_lexico.linha;
    conteudo.tamanho = _tamanho(valor_lexico, tipo, tamanho_vetor);
    conteudo.tipo = tipo;
    conteudo.natureza = natureza;
    conteudo.argumentos = NULL;
    conteudo.valor_lexico = _malloc_copia_vlex(valor_lexico);
    return conteudo;
}

Conteudo _novo_conteudo_literal(ValorLexico valor_lexico, Tipo tipo) {
    return _novo_conteudo(valor_lexico, tipo, NATUREZA_LITERAL, 0);
}

//#endregion Auxiliares

//#region Busca

//  função que retorna uma entrada específica da hash a partir de sua chave
EntradaHash *_busca_pilha(char *chave) {

    PilhaHash *pilha = global_pilha_hash; 

    EntradaHash *busca = NULL;

    while(pilha != NULL) {

        busca = _busca_topo_pilha(chave, pilha);

        if(busca != NULL) return busca;
        
        pilha = pilha->resto;
    }
	
    return busca;
}

EntradaHash *_busca_topo_pilha(char *chave, PilhaHash *pilha) {

    EntradaHash *tabela = pilha->topo;

    int indice = _indice_hash(chave) % TAMANHO_INICIAL_HASH;  

    while(tabela[indice].chave != NULL) {

        if(compare_eq_str(tabela[indice].chave, chave))
            return &tabela[indice]; 
            
        indice = _probing(indice, pilha->tamanho_tabela);
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
    global_ultima_funcao = resposta->conteudo.valor_lexico.label;
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

// identificador = variavel ou vetor
void _insere_identificador_sem_tipo_pilha(char* chave, int tamanho_vetor) {

    VariavelSemTipoLst *nova_vst;
    nova_vst = malloc(sizeof(VariavelSemTipoLst));
    nova_vst->chave = chave;
    nova_vst->tamanho_vetor = tamanho_vetor;

    VariavelSemTipoLst *vst_head = global_variaveis_sem_tipo;
    nova_vst->proximo = vst_head;
    global_variaveis_sem_tipo = nova_vst;
}

void insere_tipo_identificador_pilha(TipoSimbolo tipo) {
    
    if(global_pilha_hash == NULL) empilha();

    PilhaHash* pilha = global_pilha_hash;

    VariavelSemTipoLst *vst = global_variaveis_sem_tipo;

    while(vst != NULL) {

       EntradaHash *busca = _busca_topo_pilha(vst->chave, pilha);

        if(busca == NULL) throwUnexpectedError(-1, ">> Não tá encontrando o identificador para atualizar o tipo pendente.");

        else {

            _verifica_vetor_string(tipo, busca);

            busca->conteudo.tipo = tipo;
            busca->conteudo.tamanho = _tamanho(busca->conteudo.valor_lexico, tipo, vst->tamanho_vetor);

            if(print_simbolos) print_pilha();
        }

        VariavelSemTipoLst *antigo_vst = vst;
        vst = vst->proximo;

        _libera_head_vst(antigo_vst);
    }

    global_variaveis_sem_tipo = vst;
}

void insere_parametro_sem_funcao(TipoSimbolo tipo, ValorLexico valor_lexico) {
    
    _declara_em_escopo(NATUREZA_VARIAVEL, tipo, valor_lexico, 0);

    ArgumentoFuncaoLst *novo_arg_lst;

    novo_arg_lst = malloc(sizeof(ArgumentoFuncaoLst));
    novo_arg_lst->tipo = tipo;

    if(global_pilha_hash == NULL) throwUnexpectedError(valor_lexico.linha, ">> Não tá empilhando antes dos parâmetros.");

    //novo ArgumentoFuncaoLst no inicio da lista de argumentos sem funcao do escopo
    ArgumentoFuncaoLst *antigo_arg_lst = global_parametros_sem_funcao;
    novo_arg_lst->proximo = antigo_arg_lst;
    global_parametros_sem_funcao = novo_arg_lst;
}

// função que adiciona um argumento à lista de argumentos de uma função
void adiciona_parametros_escopo_anterior(Nodo *nodo_funcao) {

    PilhaHash *pilha = global_pilha_hash;

    if(pilha == NULL) {
        throwUnexpectedError(nodo_funcao->valor_lexico.linha, ">> Sem escopo anterior para adicionar os parâmetros.");
    }

    if(global_parametros_sem_funcao == NULL) return;

    char *chave_malloc = _chave(nodo_funcao->valor_lexico);

    EntradaHash *busca = _busca_topo_pilha(chave_malloc, pilha->resto);

    if(busca != NULL) {
        _verifica_parametros_funcao(global_parametros_sem_funcao, busca);
        
        busca->conteudo.argumentos = reverse_args(global_parametros_sem_funcao);

        global_parametros_sem_funcao = NULL;
    } else {    
        throwUnexpectedError(nodo_funcao->valor_lexico.linha, ">> Sem função para adicionar os parâmetros.");
    }

    free(chave_malloc);
}

ArgumentoFuncaoLst* reverse_args(ArgumentoFuncaoLst* head) {

    if(head == NULL || head->proximo == NULL) return head;

    ArgumentoFuncaoLst* rest = reverse_args(head->proximo);

    head->proximo->proximo = head;

    head->proximo = NULL;

    return rest;
}

//função que faz deep copy para evitar double free() nos vlex compartilhados pela AST e tabela de símbolos!
ValorLexico _malloc_copia_vlex(ValorLexico valor_lexico) {

    ValorLexico new_vlex = valor_lexico;
    new_vlex.label = strdup(valor_lexico.label);

    if(tem_valor_string(valor_lexico)) {
        new_vlex.valor_string = strdup(valor_lexico.valor_string);
    }

    return new_vlex;
}

// função que adiciona uma entrada na hash e retorna a recém-adicionada entrada
EntradaHash *_declara_em_escopo(NaturezaSimbolo natureza, TipoSimbolo tipo, ValorLexico valor_lexico, int tamanho_vetor) {

    if(global_pilha_hash == NULL) empilha();

    PilhaHash* pilha = global_pilha_hash;

    if(pilha == NULL) return NULL;

    char *chave_malloc = _chave(valor_lexico);

    EntradaHash *busca = _busca_topo_pilha(chave_malloc, pilha);

    if(busca != NULL) {
        free(chave_malloc);
        throwDeclaredError(valor_lexico.linha, valor_lexico.label, busca->conteudo.linha);
        return NULL;
    }

    Conteudo conteudo = _novo_conteudo(valor_lexico, tipo, natureza, tamanho_vetor);

    EntradaHash *resposta = _insere_topo_pilha(chave_malloc, pilha, conteudo);

    if(print_simbolos) {
        printf("\n>> OP: DECLARAÇÃO\n");
        print_pilha();
    }

    return resposta;
}

// se já existir não insere dnv, mas tbm n dá erro. dá pra botar todos literais no escopo global se quiser.
EntradaHash *_declara_literal_em_escopo(TipoSimbolo tipo, ValorLexico valor_lexico) {

    if(global_pilha_hash == NULL) empilha();

    PilhaHash* pilha = global_pilha_hash;

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

    int indice = _indice_hash(chave) % TAMANHO_INICIAL_HASH; //o primeiro indice sempre tem q ser com o tamanho inicial da tabela!

    while(tabela != NULL) {

        if(tabela[indice].chave == NULL) {

            tabela[indice].chave = chave;
            tabela[indice].conteudo = conteudo;

            pilha->quantidade_atual++;
            
            _verifica_ocupacao_tabela(pilha);

            return &tabela[indice]; 
        }

        indice = _probing(indice, tamanho_tabela);
    }
}

// função que "empilha" uma nova hash em cima da atual
void empilha()
{
    PilhaHash *pilha_aux;
    pilha_aux = malloc(sizeof(PilhaHash));

    pilha_aux->quantidade_atual = 0;
    pilha_aux->tamanho_tabela = TAMANHO_INICIAL_HASH;

    EntradaHash *tabela = _malloc_tabela();
    pilha_aux->topo = tabela;
    pilha_aux->resto = global_pilha_hash;

    global_pilha_hash = pilha_aux;

    if(print_simbolos) printf("\n>> OP: EMPILHANDO\n");
}

//aloca novo array de EntradaHash (com valores NULL pls)
EntradaHash *_malloc_tabela() {

    EntradaHash *tabela = malloc(sizeof(EntradaHash) * TAMANHO_INICIAL_HASH);

    for (int i = 0; i < TAMANHO_INICIAL_HASH; i++) {
        EntradaHash* entrada = &tabela[i];
        _inicializa_entrada(entrada);
    }

    return tabela;
}

void _inicializa_entrada(EntradaHash *entrada) {
    entrada->chave = NULL;
    entrada->conteudo.tipo = -1;
    entrada->conteudo.natureza = -1;
    entrada->conteudo.linha = -1;
    entrada->conteudo.tamanho = -1;
    entrada->conteudo.argumentos = NULL;
}

//#endregion Insere

//#region Libera 

void libera_pilha() {

    _libera_vsts();

    while(global_pilha_hash != NULL) {
        desempilha();
    }
}

// função que "desempilha" (elimina) a hash e suas informações no topo da pilha
void desempilha()
{
    if(global_pilha_hash == NULL) return;

    PilhaHash *nova_pilha = global_pilha_hash->resto;

    PilhaHash *antiga_pilha = global_pilha_hash;

    _libera_tabela(antiga_pilha->topo, antiga_pilha->tamanho_tabela);

    free(antiga_pilha);

    global_pilha_hash = nova_pilha;

    if(print_simbolos) printf("\n>> OP: DESEMPILHANDO\n");
}

// função que libera a tabela hash e tudo que há dentro dela
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

// só aponta mesmo. o conteúdo vai ser desalocado ao liberar todas as chaves das tabelas
void libera_ultima_funcao() {
    global_ultima_funcao = NULL;
}

// libera a head da lista global de variáveis sem tipo (também chamada quando encontramos o tipo)
void _libera_head_vst(VariavelSemTipoLst *vst) {

    if(vst == NULL) return;

    if(vst->chave != NULL)
        free(vst->chave);

    free(vst);
}

// libera lista global de variáveis sem tipo (por precaução, já é pra estar vazia.)
void _libera_vsts() {

    VariavelSemTipoLst *vst = global_variaveis_sem_tipo;

    while(vst != NULL) {

        _libera_head_vst(vst);

        vst = global_variaveis_sem_tipo->proximo;

        global_variaveis_sem_tipo = vst;
    }
}

//libera argumentos de uma funcao na tabela
void _libera_argumentos(ArgumentoFuncaoLst *argumento) {

    if(argumento == NULL) return;

    _libera_argumentos(argumento->proximo);

    free(argumento);
}

//#endregion Libera 

//#region Verificação

void verifica_atribuicao(Nodo *esq, Nodo *operador, Nodo *dir) {

    _verifica_conversao_implicita(esq->tipo, esq->valor_lexico, dir->tipo, dir->valor_lexico, 0);

    operador->tipo = esq->tipo;
}

void verifica_return(Nodo *operador, Nodo *expr1) {

    if(global_ultima_funcao != NULL) {

        char* chave = _chave_label(global_ultima_funcao);

        EntradaHash *busca_funcao = _busca_pilha(chave);

        free(chave);

        if(busca_funcao != NULL) {
            
            if(busca_funcao->conteudo.tipo != expr1->tipo) {
                
                if(expr1->tipo == TIPO_STRING) {
                    throwFunctionStringError(expr1->valor_lexico.linha, expr1->valor_lexico.label);
                }

                if(busca_funcao->conteudo.tipo == TIPO_CHAR) {
                    throwReturnError(expr1->valor_lexico.linha, expr1->valor_lexico.label);
                }
            }
            operador->tipo = busca_funcao->conteudo.tipo;
            return;
        }
    }
    throwReturnError(expr1->valor_lexico.linha, expr1->valor_lexico.label);
}

void verifica_expr_ternaria(Nodo *validacao, Nodo *expr1, Nodo *expr2, Nodo *operador) {

    _verifica_op_str_char_erro(validacao, operador->valor_lexico);

    Tipo tipo = get_tipo_inferencia(expr1, expr2);

    operador->tipo = tipo;
}

void verifica_expr_binaria(Nodo *esq, Nodo *operador, Nodo *dir) {

    _verifica_op_str_char_erro(esq, operador->valor_lexico);

    _verifica_op_str_char_erro(dir, operador->valor_lexico);

    Tipo tipo = get_tipo_inferencia(esq, dir);

    operador->tipo = tipo;
}

void verifica_expr_unaria(Nodo *nodo_unario, Nodo *nodo) {

    _verifica_op_str_char_erro(nodo, nodo_unario->valor_lexico);

    if(nodo_unario != NULL)
        nodo_unario->tipo = nodo->tipo;
}

void verifica_input(Nodo *nodo_input, Nodo *nodo) {
    if(nodo->tipo != TIPO_INT && nodo->tipo != TIPO_FLOAT) {
        throwWrongParInput(nodo->valor_lexico.linha, nodo->valor_lexico.label);
    }

    nodo_input->tipo = nodo->tipo;
}

void _verifica_op_str_char_erro(Nodo *nodo, ValorLexico vlex_op) {
    if(nodo->tipo == TIPO_STRING) {
        throwStringToXError(nodo->valor_lexico.linha, nodo->valor_lexico.label, vlex_op.label);
    }
    if(nodo->tipo == TIPO_CHAR) {
        throwCharToXError(nodo->valor_lexico.linha, nodo->valor_lexico.label, vlex_op.label);
    }
}

void verifica_output(Nodo *nodo_output, Nodo *nodo) {
    if(nodo->tipo != TIPO_INT && nodo->tipo != TIPO_FLOAT) {
        throwWrongParOutput(nodo->valor_lexico.linha, nodo->valor_lexico.label);
    }

    nodo_output->tipo = nodo->tipo;
}

void verifica_shift(Nodo *nodo_shift, ValorLexico valor_lexico_int) {
    if(valor_lexico_int.valor_int > 16) {
        throwWrongParShift(valor_lexico_int.linha);
    }

    nodo_shift->tipo = TIPO_INT;
}

void verifica_inicializacao(Tipo tipo, Nodo *nodos_inicializados) {

    Nodo *nodo_operacao = nodos_inicializados;
    Nodo *nodo_esq, *nodo_dir;

    while(nodo_operacao != NULL) {

        nodo_esq = nodo_operacao->filho;

        if(nodo_esq == NULL) break;

        nodo_dir = nodo_esq->irmao;

        if(nodo_dir == NULL) break;

        _verifica_conversao_implicita(tipo, nodo_esq->valor_lexico, nodo_dir->tipo, nodo_dir->valor_lexico, 1);

        nodo_operacao->tipo = nodo_esq->tipo = tipo;

        nodo_operacao = nodo_dir->irmao;
    }
}

void _verifica_parametros_funcao(ArgumentoFuncaoLst *parametros, EntradaHash *entrada_funcao) {

    ArgumentoFuncaoLst *params = parametros;

    while(params != NULL) {

        if(params->tipo == TIPO_STRING) throwFunctionStringError(entrada_funcao->conteudo.linha, "parâmetros");
        
        params = params->proximo;
    }
}

// em expr1 (= / <=) expr2: expr1 é esq, expr2 é dir.
void _verifica_conversao_implicita(Tipo tipo_esq, ValorLexico esq, Tipo tipo_dir, ValorLexico dir, int inicializacao) {

    if(tipo_esq != tipo_dir) {

        if(tipo_esq == TIPO_STRING || tipo_esq == TIPO_CHAR) {
            throwWrongTypeError(dir.linha, dir.label, esq.label, _tipo_str(tipo_esq));
        }

        //TODO ver se tem um jeito de resolver aquele problem dos kal73, kal74
        if(tipo_dir == TIPO_STRING) {
            throwStringToXError(dir.linha, dir.label, esq.label);
        }
        
        if(tipo_dir == TIPO_CHAR) {
            throwCharToXError(dir.linha, dir.label, esq.label);
        }

        //tive que fazer um compromisso e lançar um erro generico ao inves do requerido em teste abc12.
        if(tipo_dir == TIPO_OUTRO) {
            throwWrongTypeError(dir.linha, dir.label, esq.label, _tipo_str(tipo_esq));
        }

    } else _verifica_tamanho_maximo_string(tipo_dir, esq, dir, inicializacao);
}
 
void _verifica_tamanho_maximo_string(Tipo tipo_dir, ValorLexico esq, ValorLexico dir, int inicializacao) {

     if(tipo_dir == TIPO_STRING) {

        char* chave_esq = _chave(esq);
        char* chave_dir = _chave(dir);

        EntradaHash *busca_esq = _busca_pilha(chave_esq);
        EntradaHash *busca_dir = _busca_pilha(chave_dir);

        if(busca_esq != NULL && busca_dir != NULL) {
                
            if(inicializacao) { //senao é atribuicao
                busca_esq->conteudo.tamanho = busca_dir->conteudo.tamanho;

                if(print_simbolos) print_pilha();

            } else if(busca_esq->conteudo.tamanho < busca_dir->conteudo.tamanho) {
                throwStringSizeError(busca_dir->conteudo.valor_lexico.linha, busca_dir->conteudo.valor_lexico.label, busca_esq->conteudo.linha);
            }
        }
        free(chave_esq);
        free(chave_dir);
    }
}

void _verifica_vetor_string(TipoSimbolo tipo, EntradaHash *entrada) {
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

    Nodo *passados_nodo = args_passados;
    ArgumentoFuncaoLst *declarados_afl = args_declarados;

    while (passados_nodo != NULL) {

        if(passados_nodo->tipo != declarados_afl->tipo) {

            if(possui_tipo(passados_nodo->tipo, declarados_afl->tipo, TIPO_STRING)) {
                throwFunctionStringError(passados_nodo->valor_lexico.linha, passados_nodo->valor_lexico.label);
            }

            if(possui_tipo(passados_nodo->tipo, declarados_afl->tipo, TIPO_CHAR)) {
                throwWrongTypeArgsError(passados_nodo->valor_lexico.linha, passados_nodo->valor_lexico.label, linha_declarada);
            }
        }
        passados_nodo = passados_nodo->irmao;
        declarados_afl = declarados_afl->proximo;
    }
}
//#endregion Verificação

//#region Prints
void print_pilha() {

    PilhaHash *aux_pilha = global_pilha_hash;

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

        aux_pilha = aux_pilha->resto;
    }
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
            aux = aux->proximo;
        }

        printf("|");
    }
    printf("\n");
}
//#endregion Prints