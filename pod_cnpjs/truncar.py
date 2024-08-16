def truncar_arquivo_csv(input_file, output_file, num_linhas=50000):
    with open(input_file, 'r', encoding='utf-8') as arquivo_entrada:
        with open(output_file, 'w', encoding='utf-8') as arquivo_saida:
            for i, linha in enumerate(arquivo_entrada):
                if i < num_linhas:
                    arquivo_saida.write(linha)
                else:
                    break

# Nome do arquivo original e do novo arquivo
arquivo_original = 'Empresas1.EMPRECSV'
novo_arquivo = 'arquivo_truncado.EMPRECSV'

# Chamando a função para truncar o arquivo
truncar_arquivo_csv(arquivo_original, novo_arquivo, 50000)

print(f"Arquivo truncado criado: {novo_arquivo}")

