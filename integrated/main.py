import subprocess
import threading
import time

def read_stdout(process):
    for line in iter(process.stdout.readline, ''):
        if line:
            print("STDOUT:", line.strip())

def read_stderr(process):
    for line in iter(process.stderr.readline, ''):
        if line:
            print("STDERR:", line.strip())

def main():
    # Start subprocess
    process = subprocess.Popen(
        ["./nimbus"],
        stdin=subprocess.PIPE,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        text=True,
        bufsize=1
    )

    # Start threads to read stdout and stderr
    threading.Thread(target=read_stdout, args=(process,), daemon=True).start()
    threading.Thread(target=read_stderr, args=(process,), daemon=True).start()

    commands = [
    "showTables",
    "showDatabases",
    "createDatabase.mydb",
    "createDatabase.testdb",
    "createDatabase.prodDB",
    "createDatabase.demo",
    "createDatabase.analytics",
    "createDatabase.inventory",
    "useDatabase.mydb",
    "useDatabase.testdb",
    "useDatabase.prodDB",
    "db.createTable.(name:stx,sname:char[20],cpi:double,marks:int)",
    "db.createTable.(name:users,uname:char[30],age:int,email:char[40])",
    "db.createTable.(name:products,pid:int,pname:char[50],price:double)",
    "db.createTable.(name:orders,orderId:int,userId:int,amount:double)",
    "db.createTable.(name:employee,eid:int,ename:char[30],dept:char[20])",
    "db.insert.(name:stx,arjun,9.1,99)",
    "db.insert.(name:users,alice,30,alice@example_com)",  # Removed dot
    "db.insert.(name:products,101,widget,19.99)",
    "db.insert.(name:orders,1001,1,250.50)",
    "db.insert.(name:employee,2001,bob,Sales)",
    "db.find.(name:stx,{sname==arjun})",
    "db.find.(name:users,{uname==alice})",
    "db.find.(name:products,{price==19.99})",
    "db.find.(name:orders,{orderId==1001})",
    "db.find.(name:employee,{ename==bob})",
    "db.update.(name:stx,{sname==arjun}$:{sname:arjun2})",
    "db.update.(name:users,{uname==alice}$:{email:alice_new@example_com})",  # Removed dot
    "db.update.(name:products,{pid==101}$:{price:17.99})",
    "db.update.(name:orders,{orderId==1001}$:{amount:275.00})",
    "db.update.(name:employee,{ename==bob}$:{dept:Marketing})",
    "db.delete.(name:stx,{sname==arjun})",
    "db.delete.(name:products,{pid==101})",
    "db.delete.(name:orders,{orderId==1001})",
    "db.delete.(name:employee,{eid==2001})",
    "db.insert.(name:stx,ram,8.7,85)",
    "db.find.(name:stx,{sname==ram})",
    "db.update.(name:stx,{sname==ram}$:{marks:90})",
    "db.delete.(name:users,{age==30})",
    "db.insert.(name:users,bob,25,bob@example_com)",  # Removed dot
    "db.find.(name:users,{uname==bob})",
    "db.update.(name:users,{uname==bob}$:{age:26})",
    "db.delete.(name:users,{uname==bob})",
    "useDatabase.demo",
    "db.createTable.(name:logs,logId:int,logMessage:char[100])",
    "db.insert.(name:logs,1,Init database)",
    "db.find.(name:logs,{logId==1})",
    "db.update.(name:logs,{logId==1}$:{logMessage:Database Initialized})",
    "db.delete.(name:logs,{logId==1})",
    "createDatabase.companyDB",
    "useDatabase.companyDB",
    "db.createTable.(name:employees,eid:int,ename:char[30],salary:double)",
    "db.insert.(name:employees,3001,carol,75000)",
    "db.find.(name:employees,{ename==carol})",
    "db.update.(name:employees,{eid==3001}$:{salary:80000})",
    "db.delete.(name:employees,{eid==3001})",
    "useDatabase.analytics",
    "db.createTable.(name:sales,salesId:int,amount:double,date:char[10])",
    "db.insert.(name:sales,4001,1500.50,20250415)",
    "db.find.(name:sales,{salesId==4001})",
    "db.update.(name:sales,{salesId==4001}$:{amount:1600.75})",
    "db.delete.(name:sales,{salesId==4001})",
    "useDatabase.inventory",
    "db.createTable.(name:stock,item:char[30],quantity:int)",
    "db.insert.(name:stock,Notebook,50)",
    "db.find.(name:stock,{item==Notebook})",
    "db.update.(name:stock,{item==Notebook}$:{quantity:45})",
    "db.delete.(name:stock,{item==Notebook})",
    "createDatabase.hr",
    "useDatabase.hr",
    "db.createTable.(name:staff,sid:int,sname:char[25],role:char[20])",
    "db.insert.(name:staff,5001,david,Manager)",
    "db.find.(name:staff,{sname==david})",
    "db.update.(name:staff,{sid==5001}$:{role:Director})",
    "db.delete.(name:staff,{sid==5001})",
    "showTables",
    "showDatabases",
    "createDatabase.finance",
    "useDatabase.finance",
    "db.createTable.(name:accounts,accId:int,holder:char[30],balance:double)",
    "db.insert.(name:accounts,6001,emma,4500.00)",
    "db.find.(name:accounts,{holder==emma})",
    "db.update.(name:accounts,{accId==6001}$:{balance:5000.00})",
    "db.delete.(name:accounts,{accId==6001})",
    "createDatabase.research",
    "useDatabase.research",
    "db.createTable.(name:studies,sid:int,title:char[50],field:char[30])",
    "db.insert.(name:studies,7001,Quantum Mechanics,Physics)",
    "db.find.(name:studies,{field==Physics})",
    "db.update.(name:studies,{sid==7001}$:{title:Advanced Quantum Mechanics})",
    "db.delete.(name:studies,{sid==7001})",
    "db.insert.(name:users,george,35,george@example_com)",  # Removed dot
    "db.find.(name:users,{uname==george})",
    "db.update.(name:users,{uname==george}$:{age:36})",
    "db.delete.(name:users,{uname==george})",
    "useDatabase.prodDB",
    "db.createTable.(name:clients,cid:int,cname:char[40],contact:char[20])",
    "db.insert.(name:clients,8001,delta Inc,1234567890)",
    "db.find.(name:clients,{cname==delta Inc})",
    "db.update.(name:clients,{cid==8001}$:{contact:0987654321})",
    "db.delete.(name:clients,{cid==8001})",
    "useDatabase.demo",
    "db.createTable.(name:logs,logId:int,description:char[100])",
    "db.insert.(name:logs,2,Update applied)",
    "db.find.(name:logs,{logId==2})",
    "db.update.(name:logs,{logId==2}$:{description:Update complete})",
    "db.delete.(name:logs,{logId==2})",
    "createDatabase.backupDB",
    "useDatabase.backupDB",
    "db.createTable.(name:backup,backupId:int,timestamp:char[20])",
    "db.insert.(name:backup,9001,20250415T1000)",
    "db.find.(name:backup,{backupId==9001})",
    "db.update.(name:backup,{backupId==9001}$:{timestamp:20250415T1100})",
    "db.delete.(name:backup,{backupId==9001})",
    "useDatabase.companyDB",
    "db.insert.(name:employees,3002,frank,68000)",
    "db.find.(name:employees,{ename==frank})",
    "db.update.(name:employees,{eid==3002}$:{salary:70000})",
    "db.delete.(name:employees,{eid==3002})",
    "showTables"
]
    
    commands
    
    
    for cmd in commands:
        print(f"Sending:{cmd}")
        process.stdin.write(cmd + "\n")
        process.stdin.flush()
        time.sleep(1)

    # Let it run a bit to print output
    time.sleep(3)

    # Close input and wait for process to finish
    process.stdin.close()
    process.wait()
    print("Subprocess finished.")

if __name__ == "__main__":
    main()
