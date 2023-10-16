#include <Windows.h>
#include <winsvc.h>
#include <string>
#include <iostream>
#include <tchar.h>
#pragma comment(lib,"kernel32.lib")
#pragma comment(lib,"advapi32.lib")

using namespace std;

class cFuncoes
{
private:

	SC_HANDLE Identificador;

	SERVICE_STATUS_PROCESS Status;

	SERVICE_CONTROL_STATUS_REASON_PARAMS Parametros;

public:

	bool SoliciarAoGerenciador()
	{
		bool bRetorno = false;

		Identificador = OpenSCManager(0, 0, SC_MANAGER_ALL_ACCESS);

		if (Identificador == NULL)
		{
			cout << "Não foi possível autenticar ao servidor de serviços do sistema..\n";
		}
		else
		{
			bRetorno = true;
		}

		return bRetorno;
	}

	void AlterarStatusDoService(wstring Nome, bool Iniciar)
	{
		SC_HANDLE Servico = OpenService(Identificador, Nome.c_str(), SERVICE_ALL_ACCESS);
		if (Servico == NULL)
		{
			cout << "Ocorreu um erro durante a operação..\n";
		}
		else
		{
			DWORD BytesRetornados;

			//Iremos chamar a função duas vezes, para alocar os bytes necessários para a segunda função e retornar com sucesso o serviço ativo.
			QueryServiceStatusEx(Servico, SC_STATUS_PROCESS_INFO, 0, 0, &BytesRetornados);
			QueryServiceStatusEx(Servico, SC_STATUS_PROCESS_INFO, (LPBYTE)&Status, BytesRetornados, &BytesRetornados);

			if (Iniciar)
			{
				//Iniciando um serviço inativo.
				StartService(Servico, 0, 0);
			}
			else
			{
				//Parando um serviço ativo.
				ControlService(Servico, SERVICE_CONTROL_STOP, (LPSERVICE_STATUS)&Status);
			}

			//Iremos finalizar os identificadores pois não será mais necessário.
			CloseServiceHandle(Servico);
			CloseServiceHandle(Identificador);
		}
	}

}Funcoes;

int main()
{

	cout << "O assistente está executando operações de status no gerenciador de serviços...\n";

	if (Funcoes.SoliciarAoGerenciador() == true)
	{
		cout << "Sucesso ao solicitar..\n";
		cout << "Operação de alteração de status em andamento..\n\n";
		
		Funcoes.AlterarStatusDoService(L"SeuServiço", true);
	}

	system("pause");
}