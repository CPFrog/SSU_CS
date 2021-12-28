class LabelTable:
    """symbol, literal과 관련된 데이터와 연산을 소유한다.
    Control Section별로 별도의 테이블을 가질 수 있도록 설계한다."""

    def __init__(self):
        self.label = list()
        self.locationList = list()

    def search(self, label_name: str) -> int:
        """인자로 전달된 symbol, literal이 어떤 주소를 가리키고 있는지 반환

        label_name:
            검색하려는 symbol/literal의 이름
        returns:
            테이블에 있는 symbol/literal일 경우 주소값을, 아닐 경우 -1을 반환.
        """
        address: int = -1
        try:
            idx = self.label.index(label_name)
            address = self.locationList[idx]
        except ValueError:
            pass
        return address

    def putName(self, new_name: str, location: int):
        """ 새로운 symbol/literal을 table에 추가한다.

        new_name:
            새로 추가되는 symbol/literal의 이름
        location:
            해당 symbol/literal이 테이블에 저장될 때 가지는 최초 주소값. 만약 값의 변경이 필요할 경우 별도의 함수에서 주소값의 변경을 처리한다.
        """
        if self.search(new_name) == -1:
            self.label.append(new_name)
            self.locationList.append(location)

    def modifyName(self, label_name: str, new_location: int):
        """기존에 존재하는 symbol/literal 의 주소값을 변경한다.
        label_name:
            변경하려는 symbol/literal의 이름
        new_location:
            바꾸려고 하는 주소값
        """
        try:
            idx: int = self.label.index(label_name)
            self.locationList[idx] = new_location
        except ValueError:
            pass
