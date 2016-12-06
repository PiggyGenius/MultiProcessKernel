    .text
# cette directive sert a rendre l'etiquette publique
    .globl manager
# debut du traitant 
manager:
# sauvegarde des registres importants
    pushl %eax
    pushl %edx
    pushl %ecx
# appel a la fonction C realisant le traitant
# a decommenter quand vous l'aurez implantee
	call clock_tick
# restauration des registres importants
    popl %ecx
    popl %edx
    popl %eax
# fin du traitant
    iret

