#pragma once

//=====================================================
//
// �E�B���h�E
//
//=====================================================
class Window {
public:

	//===================================
	// �擾�E�ݒ�
	//===================================

	// �E�B���h�E�n���h���擾
	HWND GetWndHandle() const { return m_hWnd; }
	// �E�B���h�E�����݂���H
	bool IsCreated() const { return m_hWnd ? true : false; }
	// �}�E�X�z�C�[���̕ω��ʂ��擾
	int GetMouseWheelVal() const { return m_mouseWheelVal; }
	// �C���X�^���X�n���h���擾
	HINSTANCE GetInstanceHandle() const { return GetModuleHandle(0); }

	// �N���C�A���g�T�C�Y�̐ݒ�
	void SetClientSize(int w, int h);

	//===================================
	// �������E���
	//===================================

	// �E�B���h�E�쐬
	bool Create(int clientWidth, int clientHeight, std::string_view titleName, std::string_view windowClassName);

	// ���
	void Release();

	//
	~Window() {
		Release();
	}

	//===================================
	// ����
	//===================================

	// �E�B���h�E���b�Z�[�W����������
	//  �߂�l�F�I�����b�Z�[�W��������false���Ԃ�
	bool ProcessMessage();

	// �t�@�C�����J���_�C�A���O�{�b�N�X��\��
	// �Efilepath		�c �I�����ꂽ�t�@�C���p�X������
	// �Etitle			�c �E�B���h�E�̃^�C�g������
	// �Efilters		�c �w�肳�ꂽ�g���q�̂ݕ\�������悤�ɂȂ�
	static bool OpenFileDialog(std::string& filepath, std::string_view title = "�t�@�C�����J��", const char* filters = "�S�Ẵt�@�C��\0*.*\0");

	// �t�@�C���������ĕۑ��_�C�A���O�{�b�N�X��\��
	// �Efilepath		�c �I�����ꂽ�t�@�C���p�X������
	// �Etitle			�c �E�B���h�E�̃^�C�g������
	// �Efilters		�c �w�肳�ꂽ�g���q�̂ݕ\�������悤�ɂȂ�
	// �EdefExt			�c ���[�U�[���g���q����͂��Ȃ������ꍇ�A���ꂪ�����I�ɕt��
	static bool SaveFileDialog(std::string& filepath, std::string_view title = "�t�@�C����ۑ�", const char* filters = "�S�Ẵt�@�C��\0*.*\0", std::string_view defExt = "");

private:

	// �E�B���h�E�n���h��
	HWND	m_hWnd = nullptr;

	// �}�E�X�z�C�[���l
	int		m_mouseWheelVal = 0;

	// �E�B���h�E�֐�
	static LRESULT CALLBACK callWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);


};
