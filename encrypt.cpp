/*
ABSTRACT:
Roblox's new luau bytecode uses a form of encryption called "2,4BRAESSCRYPT", basically, it uses mathematical derivatives (from calculus)
to encrypt their bytecode structure. In the deserializer, they use simple functions to decrypt

What we need to do: encrypt it so the decrypter can read it!
*/

double cubicFormula(int a, int b, int c, int d) {
	return (std::cbrt((-std::pow(b, 3) / (27 * std::pow(a, 3))) + ((b*c) / (6 * std::pow(a, 2))) - (d / (2 * a)) + std::sqrt(
			std::pow( (-std::pow(b, 3) / (27 * std::pow(a, 3))) + ((b*c) / (6 * std::pow(a, 2))) - (d / (2 * a)), 2) + std::pow((c / (3 * a)) - (std::pow(b, 2) / (9 * std::pow(a, 2))), 3)))
		+
		std::cbrt((-std::pow(b, 3) / (27 * std::pow(a, 3))) + ((b*c) / (6 * std::pow(a, 2))) - (d / (2 * a)) - std::sqrt(
			std::pow((-std::pow(b, 3) / (27 * std::pow(a, 3))) + ((b*c) / (6 * std::pow(a, 2))) - (d / (2 * a)), 2) + std::pow((c / (3 * a)) - (std::pow(b, 2) / (9 * std::pow(a, 2))), 3)))
	-(b / (3 * a)));
}

double quadraticFormula(int a, int b, int c) {
	return (
		-b + (std::sqrt(std::pow(b, 2) - (4 * a*c)) / 2 * a)
		+
		-b - (std::sqrt(std::pow(b, 2) - (4 * a*c)) / 2 * a)
		);
}

int encryptABx(Instruction OP) {
  Instruction enc = 0;
  double A = (OP >> 8);
  double Bx = (OP >> 16);
  double Op = (OP & 0xFF);
  enc = quadraticFormula(Op, A, Bx);
  return enc;
}

int encryptABC(Instruction OP) {
  Instruction enc = 0;
  double A = (OP >> 8);
  double B = (OP >> 16);
  double C = (OP >> 24);
  double Op = (OP & 0xFF);
  enc = cubicFormula(Op, A, B, C);
  return enc;
}


/*

USAGE EXAMPLE

switch (GET_OPCODE(inst)) { // usual luau case switch
	case OP_RETURN: {
		Instruction raw = generateABC(GETARG_A(inst), GETARG_B(inst), GETARG_C(inst)); // generate our luau-formatted ABC instruction
		rinst = encryptABC(raw); // encrypt our raw luau opcode so it can be decrypted
		MyConvertedCode.push_back(rinst); // add our encrypted instruction to the vector, ready to be added to p->code
	break; }
}

*/
