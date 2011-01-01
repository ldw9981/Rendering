08-12-24
한 것
1.애니메이션 정보를 리소스로 떼어어 내려다 2일전 소스로 롤백
(고칠것은 많지만 수정시 수정사항에 관련된 소스만 수정하고 테스트하자)
2. Scene->SceneRoot 로 수정. SceneRoot는 일종의 SceneNode이며 ( Transform관련 메소드를 그대로 쓸수있다.)
매트리얼 리스트와 각종 씬타임정보 자식 노드의 정보들을 부가로 갖는다.
3.애니메이션의 보간도 ms기준으로 보간한다. 
4. Parser에서 SceneRoot에 Mesh를 넣을때 부모기준 local도 미리 계산한다. 
Init부분에서 하지않는다.

5. 내부 string사용은 모두 string으로 교체
6. Release후 Free실행하여 인스턴스를 delete한경우 더이상 인스턴스멤버는 엑세스할수없다 그러므로상수0리턴

할 것
1. Transform 애니메이션 time,TM정보의리소스로분리



08-12-25
한것
1.cStatic*** 유틸리티 클래스의 멤버데이터는 protected로 (상속받은것만 접근)
상속은 private로 유도클래스를 만든다. (has관계로 표현)
2. LinkToBone,LinkToParent 함수 정리
실시간으로 계산하지않아도 되는것은 미리계산해둔다.

할것
1. Transform 애니메이션 time,TM정보의리소스로분리


08-12-27
한것
1. TransformAnm의 리소스화 성공
2. cSphere::Union 기능 추가
3. cTransform 설계
할것
1. cTransform -> cWorldTransform으로 변경
2. cSceneRoot , cSceneGraphNode , cGameObject 의공통부분은 cWorldTransform 으로 대체(포함?,,상속public?private?)

08-12-28
한것
1. cSceneRoot 삭제 ,cGameObject 삭제
2. cTransformable 으로 변경
할것

1. cCameraNode,cCamera 통합
2. SceneMng,Scene의 생성


08-12-30
한것

할것
