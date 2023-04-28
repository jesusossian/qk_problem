module Data

struct InstanceData
  N::Int #number of itens
  P # profit for knapsack
  W # weight fo knapsack
  C # capacitated for knapsack
end

export InstanceData, readData

function readData(instanceFile,params)
  file = open(instanceFile)
  fileText = read(file, String)
  tokens = split(fileText) 
  #tokens will have all the tokens of the input file in a single vector. We will get the input token by token

  # read the problem dimensions N
  aux = 1
  N = parse(Int,tokens[aux])

  #resize data structures according to P and W
  P = zeros(Int,N,N)
  W = zeros(Int,N)

  for j in 1:N
    aux = aux+1
    P[j,j] = parse(Int,tokens[aux])
  end

  for i in 1:N
    for j in (i+1):N
      aux = aux+1
      P[i,j] = parse(Int,tokens[aux])
    end
  end

  aux += 1
  C = parse(Int,tokens[aux])

  for j in 1:N
    aux = aux + 1
    W[j] = parse(Int,tokens[aux])
  end

#  println("$N")
#  for j in 1:N
#    print("$(P[j,j]) ")
#  end
#  println()

#  for i in 1:N
#    for j in (i+1):N
#      print("$(P[i,j]) ")
#    end
#    println()
#  end

#  println("$C")

#  for j in 1:N
#    print("$(W[j]) ")
#  end
#  println()

  close(file)

#  open("saida.txt","a") do f
#    write(f,"$instanceFile")
#  end

  inst = InstanceData(N,P,W,C)

  return inst

end

end
