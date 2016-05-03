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
	public: System::Windows::Forms::TrackBar^  tbKludgeCoef;
	public: System::Windows::Forms::Label^  lblKludgeCoef;
	public: System::Windows::Forms::Label^  lblWindspeed;
	public: System::Windows::Forms::TrackBar^  tbWindspeed;

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
			this->tbKludgeCoef = (gcnew System::Windows::Forms::TrackBar());
			this->lblKludgeCoef = (gcnew System::Windows::Forms::Label());
			this->lblWindspeed = (gcnew System::Windows::Forms::Label());
			this->tbWindspeed = (gcnew System::Windows::Forms::TrackBar());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->tbKludgeCoef))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->tbWindspeed))->BeginInit();
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
			// tbKludgeCoef
			// 
			this->tbKludgeCoef->Location = System::Drawing::Point(9, 112);
			this->tbKludgeCoef->Maximum = 100;
			this->tbKludgeCoef->Name = L"tbKludgeCoef";
			this->tbKludgeCoef->Size = System::Drawing::Size(260, 45);
			this->tbKludgeCoef->TabIndex = 1;
			this->tbKludgeCoef->Value = 85;
			// 
			// lblKludgeCoef
			// 
			this->lblKludgeCoef->AutoSize = true;
			this->lblKludgeCoef->Location = System::Drawing::Point(9, 96);
			this->lblKludgeCoef->Name = L"lblKludgeCoef";
			this->lblKludgeCoef->Size = System::Drawing::Size(114, 13);
			this->lblKludgeCoef->TabIndex = 2;
			this->lblKludgeCoef->Text = L"Kludge Coefficient: 8.5";
			// 
			// lblWindspeed
			// 
			this->lblWindspeed->AutoSize = true;
			this->lblWindspeed->Location = System::Drawing::Point(9, 32);
			this->lblWindspeed->Name = L"lblWindspeed";
			this->lblWindspeed->Size = System::Drawing::Size(82, 13);
			this->lblWindspeed->TabIndex = 4;
			this->lblWindspeed->Text = L"Windspeed: 5.0";
			// 
			// tbWindspeed
			// 
			this->tbWindspeed->Location = System::Drawing::Point(9, 48);
			this->tbWindspeed->Maximum = 100;
			this->tbWindspeed->Name = L"tbWindspeed";
			this->tbWindspeed->Size = System::Drawing::Size(260, 45);
			this->tbWindspeed->TabIndex = 3;
			this->tbWindspeed->Value = 50;
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(284, 178);
			this->Controls->Add(this->lblWindspeed);
			this->Controls->Add(this->tbWindspeed);
			this->Controls->Add(this->lblKludgeCoef);
			this->Controls->Add(this->tbKludgeCoef);
			this->Controls->Add(this->cbWireframe);
			this->Name = L"MyForm";
			this->Text = L"Assignment Ocean";
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->tbKludgeCoef))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->tbWindspeed))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
};
}
