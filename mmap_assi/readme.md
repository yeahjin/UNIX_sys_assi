# hw4에 대한 교수님 피드백

1. exec 계열에서 첫번째 인자는 실제 실행 파일 경로이며, argv로 전달하는 첫 번째는 argv[0] 는 실행파일이름이 되도록 해야 합니다. 즉, childargv[0] = "cli" 가 되어야 하며 childargv[1] = val1; childargv[2] = val2; 와 같은 식으로 지정되어야만 합니다. 
2. sort 와 client.mmap 과 통신에서 사용한 파일과 client.mmap<->sorter.mmap 이 사용하는 파일은 같으면 여러가지 문제가 발생합니다. 이렇게 전달할 경우 client.mmap 이 존재할 이유가 없습니다. sorter.mmap 은 sort의 실행 여부와 관계없이 sorinng의 서버 역할을 할수 있어야만 합니다.