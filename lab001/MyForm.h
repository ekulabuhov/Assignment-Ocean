#pragma once

namespace lab001 {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for MyForm
	/// </summary>
	public ref class MyForm : public System::Windows::Forms::Form
	{
	public:
		MyForm(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~MyForm()
		{
			if (components)
			{
				delete components;
			}
		}
	public: System::Windows::Forms::CheckBox^  cbWireframe;
	protected: 

	protected: 







	private: 

	public: 

	private: 

	public: 

	private: 


	private: 



	private: 

	public: 



	protected: 

	protected: 




	protected:





	private:
	protected:










	public: 

	public:
	private:


	public:
	private:
	public:





	private: 




	public: 
	private: 

	public: 

	private: 

	public: 
	public: 
	private: 


	public: 
	protected: 

	protected: 

	protected: 

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->cbWireframe = (gcnew System::Windows::Forms::CheckBox());
			this->SuspendLayout();
			// 
			// cbWireframe
			// 
			this->cbWireframe->AutoSize = true;
			this->cbWireframe->Location = System::Drawing::Point(12, 12);
			this->cbWireframe->Name = L"cbWireframe";
			this->cbWireframe->Size = System::Drawing::Size(74, 17);
			this->cbWireframe->TabIndex = 0;
			this->cbWireframe->Text = L"Wireframe";
			this->cbWireframe->UseVisualStyleBackColor = true;
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(284, 158);
			this->Controls->Add(this->cbWireframe);
			this->Name = L"MyForm";
			this->Text = L"Assignment Ocean";
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
};
}
