void fac()
{
	pushi(ebp); // Setting dynamic link
	move(esp,ebp); // Setting dynamic link
	pushi(0);
	loadi(ind(ebp,4),eax);
	pushi(eax);
	move(4,edx);
	loadi(ind(esp),eax);
	loadi(ind(esp,4),ebx);
	popi(1);
	popi(1);
	cmpi(ebx,eax);
	jne(label1);
	pushi(1);
	j(label2);
label1:
	pushi(0);
label2:
	loadi(ind(esp, 0), eax);
	popi(1);
	cmpi(0,eax);
	je(label0);
	pushi(1);
	loadi(ind(esp),eax);
	popi(1);
	storei(eax,ind(ebp,8));
	j(e);
	j(e);
label0:
	pushi(0); // for _storing return value
	pushi(1);
	loadi(ind(ebp,4),eax);
	pushi(eax);
	move(4,edx);
	loadi(ind(esp),eax);
	loadi(ind(esp,4),ebx);
	popi(1);
	popi(1);
	muli(-1,ebx);
	addi(ebx,eax);
	pushi(eax);
	fac();
	popi(1);
	loadi(ind(ebp,4),eax);
	pushi(eax);
	move(4,edx);
	loadi(ind(esp),eax);
	loadi(ind(esp,4),ebx);
	popi(1);
	popi(1);
	muli(ebx,eax);
	pushi(eax);
	loadi(ind(esp),eax);
	popi(1);
	storei(eax,ind(ebp,8));
	j(e);
e:
	loadi(ind(ebp), ebp); // restoring dynamic link
	popi(1); //pop stack
	return; //return
}
void main()
{
	pushi(0); //for i storage
	pushi(4);
	loadi(ind(esp),eax);
	popi(1);
	muli(-1,eax);
	storei(eax,ind(esp));
	loadi(ind(ebp,-4),eax);
	pushi(eax);
	move(-4,edx);
	loadi(ind(esp,4),eax);
	storei(eax,ind(ebp,geti(edx)));
	popi(1);
	popi(1);
	pushi(0); // for _storing return value
	pushi(3);
	fac();
	popi(1);
	loadi(ind(ebp,-4),eax);
	pushi(eax);
	move(-4,edx);
	loadi(ind(esp,4),eax);
	storei(eax,ind(ebp,geti(edx)));
	print_int(eax); // Special method to print contents of registers
	print_char('\n');
	popi(1);
	popi(1);
	pushi(0);
	loadi(ind(esp),eax);
	popi(1);
	storei(eax,ind(ebp,4));
	j(e);
e:
	return; //return
}
