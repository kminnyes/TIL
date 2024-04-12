### 🔖length()와 length의 차이

length()와 length는 모두 길이를 알고자할 때 사용한다.

하지만 length()는 문자열(String)의 길이를 알고자할 때 사용되고,
length는 배열의 길이를 알고자할 때 사용된다.

- 예시
```
		//length
		int [] arr = new int[5];
		System.out.println(arr.length); //5

		//length()
		String str = "Abcdef";
		System.out.println(str.length()); //6
```
