// SPDX-License-Identifier: MIT
pragma solidity ^0.8.0;
contract SmartContract{
    address public owner;
    string x;
    constructor()
    {
        owner=msg.sender;
    }
    modifier onlyOwner(){
        require(msg.sender == owner,"MUST BE OWNER");   //only boss can decide x
        _;
    }
    struct CommitChoice {
        address playerAddress;
        bytes32 commitment;
        
    }
    function getx() public view returns(string memory){
        return x; //everyone can know the x input.
    }
    function setSecret(string memory _x) public onlyOwner {
        x= _x;  //deciding the x input value here.
    }
    


    
    
}
