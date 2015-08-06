package com.showmo.ormlite;

import java.sql.SQLException;
import java.util.HashMap;
import java.util.Map;

import android.accounts.Account;
import android.content.Context;
 
import android.database.sqlite.SQLiteDatabase;
import android.database.sqlite.SQLiteDatabase.CursorFactory;
import android.util.Log;

import com.j256.ormlite.android.AndroidDatabaseConnection;
import com.j256.ormlite.android.apptools.OrmLiteSqliteOpenHelper;
import com.j256.ormlite.dao.Dao;
import com.j256.ormlite.dao.DaoManager;
import com.j256.ormlite.support.ConnectionSource;
import com.j256.ormlite.support.DatabaseConnection;
import com.j256.ormlite.table.TableUtils;
import com.showmo.alarmManage.Alarm;
import com.showmo.deviceManage.Device;
import com.showmo.ormlite.model.ShowmoAccount;
import com.showmo.safe.Safe;
import com.showmo.util.LogUtils;

public class DatabaseHelper extends OrmLiteSqliteOpenHelper {
   
	//	数据库移植
//	db.execSQL(RENAME_OLD_TABLE);   //重命名旧数据库
//	TableUtils.createTableIfNotExists(conn, ShowmoAccount.class); //创建新数据库
//	db.execSQL(INSERT_DATA);   //数据迁移
//	db.execSQL(DROP_OLD_TABLE);  //销毁旧数据库
	
//	private static final String RENAME_OLD_TABLE = "alter table showmo_account rename to _temp_showmo_account";  
//	//迁移数据 字段需要对应，新字段赋默认值如 testInt --> 2  testStr --> 'aa'
//	private static final String INSERT_DATA = "insert into showmo_account(   userName  ,psssword , accountState , isSavePsw , testInt ,testStr  ) "
//			+ "select userName ,psssword , accountState , isSavePsw ,  2, 'aa'  from _temp_showmo_account";  
//	
//	private static final String DROP_OLD_TABLE = "drop table _temp_showmo_account"; 

 
	
	
    private static final String TABLE_NAME ="data.db";
    
    private static final int TABLE_VERSION =7;//数据库升级时要改变这个
    
    @SuppressWarnings("rawtypes")
	private Map<String , Dao> daos = new HashMap<String,Dao>();
 
    
    private DatabaseHelper(Context context){
        super(context, TABLE_NAME, null, TABLE_VERSION);
    }
    
	@Override
	public void onCreate(SQLiteDatabase db, ConnectionSource conn) {
		try {
			Log.e("DatabaseHelper", "onCreate");
			TableUtils.createTableIfNotExists(conn, ShowmoAccount.class);
			TableUtils.createTableIfNotExists(conn, Device.class);
			TableUtils.createTableIfNotExists(conn, Alarm.class);
			TableUtils.createTableIfNotExists(conn, Safe.class);
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
	@Override
	public void onDowngrade(SQLiteDatabase db, int oldVersion, int newVersion) {
		// TODO Auto-generated method stub
		ConnectionSource cs = getConnectionSource();
		/*
		 * The method is called by Android database helper's get-database calls when Android detects that we need to
		 * create or update the database. So we have to use the database argument and save a connection to it on the
		 * AndroidConnectionSource, otherwise it will go recursive if the subclass calls getConnectionSource().
		 */
		DatabaseConnection conn = cs.getSpecialConnection();
		boolean clearSpecial = false;
		if (conn == null) {
			conn = new AndroidDatabaseConnection(db, true, cancelQueriesEnabled);
			try {
				cs.saveSpecialConnection(conn);
				clearSpecial = true;
			} catch (SQLException e) {
				throw new IllegalStateException("Could not save special connection", e);
			}
		}
		try {
			if(oldVersion != newVersion){
    			LogUtils.e("upgrade", "SQLiteDatabase onDowngrade oldversion "+oldVersion+" newversion "+newVersion);
    			TableUtils.dropTable(cs, ShowmoAccount.class, true);
        		TableUtils.dropTable(cs, Device.class, true);
        		TableUtils.dropTable(cs, Alarm.class, true);
        		TableUtils.dropTable(cs, Safe.class, true);
        		onCreate(db,cs);
    		}
		}catch(Exception e){
			e.printStackTrace();
		}
		finally {
			if (clearSpecial) {
				cs.clearSpecialConnection(conn);
			}
		}
	}

    @Override
    public void onUpgrade(SQLiteDatabase db, ConnectionSource conn, int oldversion, int newversion) {
    	try {
    		
    		if(oldversion != newversion){
    			LogUtils.e("upgrade", "SQLiteDatabase onUpgrade oldversion "+oldversion+" newversion "+newversion);
    			TableUtils.dropTable(conn, ShowmoAccount.class,true);
    			TableUtils.dropTable(conn, Device.class,true);
    			TableUtils.dropTable(conn, Alarm.class,true);
    			TableUtils.dropTable(conn, Safe.class,true);
        		onCreate(db,conn);
    		}
		} catch (Exception e) {
			// TODO: handle exception
			e.printStackTrace();
		}
    	
    }
    
    private static DatabaseHelper instance;
    
    public static synchronized DatabaseHelper getHelper(Context context){
        
        context = context.getApplicationContext();
        
        if(instance == null){
            
            synchronized (DatabaseHelper.class) {
                if(instance == null){
                    instance = new DatabaseHelper(context);
                }
                
            }
            
        }
        
        return instance;
    }
    
    @SuppressWarnings("unchecked")
    public synchronized Dao getDao(Class clazz)throws  SQLException {
        
        Dao dao = null ;
        String className = clazz.getSimpleName();
        
        if(daos.containsKey(className)){
            
            dao = daos.get(className);
        }
        if(dao == null){
            dao = super.getDao(clazz);
            daos.put(className, dao);
          
        }
      
        return dao;
    }



    @Override
    public void close() {
        super.close();

        for(String key :daos.keySet()){
            Dao dao = daos.get(key);
            dao = null ;
        }
    }
    
     
    
}
