double optimization (int N, int **P, int *W, int C, int *elapsed) {
  string varName;
  double objval_;
  time_t start, end;
  int elapsed_;
   
  IloEnv env;

  try {
    
    IloModel model(env);
    
    IloNumVarArray x(env,N+1,0.0,1.0,ILOINT);
    
    FloatMatrix y(env,N+1);
    for(int i=0; i<N; i++) {
      y[i] = IloNumVarArray(env,N+1,0.0,1.0,ILOINT);
    }      
    
    for (int i=0; i<N; i++) {
      varName = "x("+IntToString(i)+")";
      x[i].setName(varName.c_str());
    }
    
    for (int i=0; i<N; i++) {
      for (int j=i+1; j<=N; j++) {
	varName = "y("+IntToString(i)+","+IntToString(j)+")";
	y[i][j].setName(varName.c_str());
      }
    }
    
    /*objective function*/
    IloExpr obj(env);
    for(int i=0; i<N; i++) {
      obj += P[i][i] * x[i];
      for (int j=i+1; j<N; j++) {
	obj += P[i][j] * y[i][j];
      }
    }
    model.add(IloMaximize(env, obj));
    obj.end();
    
    /*constraints*/
    IloExpr ctr(env);      
    for (int i=0; i<N; i++) {
      ctr += W[i] * x[i];
    }
    model.add(ctr <= C);
    ctr.end();

    for (int j=0; j<N; j++) {
      IloExpr ctr(env);
      for (int i=0; i<N; i++) {
    	if (i<j) ctr += W[i] * y[i][j];
    	if (i>j) ctr += W[i] * y[j][i];
      }
      ctr += (W[j] - C) * x[j];
      model.add(ctr <= 0);
      ctr.end();
    }
    
        
    for (int i=0; i<N; i++) {
      IloExpr ctr(env);
      for (int j=i+1; j<N; j++) {
	ctr = y[i][j] - x[i];
	model.add(ctr <= 0);
      }
      ctr.end();
    }
    
    for (int i=0; i<N; i++) {
      IloExpr ctr(env);
      for (int j=i+1; j<N; j++) {
	ctr = y[i][j] - x[j];
	model.add(ctr <= 0);
      }
      ctr.end();
    }
    
    for (int i=0; i<N; i++) {
      IloExpr ctr(env);
      for (int j=i+1; j<N; j++) {
	ctr = x[i] + x[j] - y[i][j];
	model.add(ctr <= 1);
      }
      ctr.end();
    }
    
    
    IloCplex cplex(model);
    cplex.exportModel("qkp.lp");
    
    /*set parameters*/
    cplex.setParam(IloCplex::EpGap, EPSILON);
    cplex.setParam(IloCplex::TiLim,MAX_CPU_TIME);
    //cplex.setParam(IloCplex::PreInd, 0);
    cplex.setParam(IloCplex::Param::Threads,1);
    
    //cplex.setParam(IloCplex::Param::RootAlgorithm, IloCplex::AutoAlg);
    cplex.setParam(IloCplex::Param::RootAlgorithm, IloCplex::Primal);
    //cplex.setParam(IloCplex::Param::RootAlgorithm, IloCplex::Dual);
    //cplex.setParam(IloCplex::Param::RootAlgorithm, IloCplex::Barrier); 
    //cplex.setParam(IloCplex::Param::Barrier::Crossover, IloCplex::NoAlg);
    //cplex.setParam(IloCplex::Param::RootAlgorithm, IloCplex::Network);  
    //cplex.setParam(IloCplex::Param::RootAlgorithm, IloCplex::Sifting);
    //cplex.setParam(IloCplex::Param::RootAlgorithm, IloCplex::Concurrent);
    
    /*solve the problem*/
    start = time(NULL);
    elapsed_ = 0;
    cplex.solve();
    end = time(NULL);
    elapsed_ = difftime(end, start);
    
    cplex.out() << "Solution status: " << cplex.getStatus() << endl;
    
    /*get solution*/
    objval_ = cplex.getObjValue();
    best_bound = cplex.getBestObjValue();
    gap = cplex.getMIPRelativeGap();
    
    for (int i = 0; i<N; i++) {
      sx[i] = cplex.getValue(x[i]);
      for (int j=i+1; j<N; j++) {
	sy[i][j] = cplex.getValue(y[i][j]);
      }
    }
    
  }
  catch (IloException& e) {
    cerr << " ERROR: " << e << endl;
  }
  catch(...) {
    cerr << " ERROR " << endl;
  }
  
  env.end();
  
  *elapsed = elapsed_;
  
  return objval_;
  
}

string IntToString (int a) {
  ostringstream temp;
  temp<<a;
  return temp.str();
}
