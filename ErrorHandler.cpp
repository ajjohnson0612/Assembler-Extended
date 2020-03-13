class ErrorHandler {
  public:
  ofstream errors;

  ErrorHandler(){
    errors.open("ErrorLog.txt");
  }

  void WriteErrors(int code, unsigned long lineNumber){
    switch(code){
      //illegal A type
      case 1:
        errors << "Illegal A type instruction at line:" << lineNumber << "\n"; 
        break;
      //illigal c tpye destination R
      case 2:
        errors << "illigal c tpye destination at line " << lineNumber << "\n";
        break;
      //illegal c type comp R
      case 3:
        errors << "illigal c tpye computation at line " << lineNumber << "\n";
        break;
      //illegal c type jump R
      case 4:
        errors << "illigal c tpye jump at line " << lineNumber << "\n";
        break;
      //illegal label
      case 5:
        errors << "illegal label at line " << lineNumber << "\n";
        break;
      // label redeclaration  R
      case 6:
        errors << "illegal redeclaration of label at line " << lineNumber << "\n";
        break;
      case 7:
        errors << "illegal .EQU at line " << lineNumber << "\n";  
        break;
      case 8:
        errors << "to large of number at line " << lineNumber << "\n";
        break;
      case 9:
        errors << "invalid syntax at line " << lineNumber << "\n";
        break;
      default :
        return;
    }

  }
};