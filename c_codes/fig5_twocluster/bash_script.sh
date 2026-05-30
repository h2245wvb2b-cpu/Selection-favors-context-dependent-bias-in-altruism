for dim in 20; do
	sed -i -e "s/PHENOTYPE_DIMENSION CHANGEDIM/PHENOTYPE_DIMENSION ${dim}/g" cooperation.h
	sed -i -e "s/execute_sim/execute_sim${dim}/g" Makefile
	make
	sed -i -e "s/PHENOTYPE_DIMENSION ${dim}/PHENOTYPE_DIMENSION CHANGEDIM/g" cooperation.h
	sed -i -e "s/execute_sim${dim}/execute_sim/g" Makefile
done
